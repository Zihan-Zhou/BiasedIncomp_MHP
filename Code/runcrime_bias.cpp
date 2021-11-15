#include "plottraj.hpp"
#include "hp.hpp"
#include "kernels.hpp"
#include "mle.hpp"
#include <string>
#include <random>
#include <fstream>
#include <iostream>
#include <limits>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

int main(int argc, char **argv) {
	traj tr;

	mleparams mlep;
	bool datas = 0;	
	int extralabels = 5;
	double initbeta = 1.0/90.0;
	double mumult = 0.1;
	double maxinitW =  initbeta/2;

	double maxT = std::numeric_limits<double>::infinity();
	double minT = 0.0;
	int allinitW = 1; // actually boolean
	unsigned int initseed = 0;
	string outputfilename = "crimedata/testout.txt";
	string outputfilename_s = "crimedata/testout_short.txt";
	string inputfilename = " ";
	double berno_rt= 1.0; //bernoulli rate to pick data
	int keep = 0; // keep=0 is random sampling; keep =40/60/80 is biased sampling;
	int top = 0;
	po::options_description odesc("options");
	odesc.add_options()
		("help","write help message and exit")
		("show","write parameters and exit")
		("Short",po::value<bool>(&datas)->default_value(0),
                        "short data considered full (unobs is empty)")
		("maxT",po::value<double>(&maxT)->default_value(std::numeric_limits<double>::infinity()),"max ending time")
		("minT",po::value<double>(&minT)->default_value(0.0),"min starting time")
		("nhidden",po::value<int>(&extralabels)->default_value(5),
			"number hidden labels")
		("nem",po::value<int>(&mlep.nsteps)->default_value(100),
			"number of EM iterations")
		("nsamp",po::value<int>(&mlep.nsamp)->default_value(50000),
			"number of MC sampler / EM iteration")
		("nburnin0",po::value<int>(&mlep.nburnin0)->default_value(100000),
			"number of initial burnin steps (per thread)")
		("nburnin",po::value<int>(&mlep.nburnin)->default_value(0),
			"number of burnin steps (per thread) after 1st EM iteration")
		("nskip",po::value<int>(&mlep.nskip)->default_value(100),
			"number of samples to skip before recording")
		("kappa",po::value<double>(&mlep.kappa)->default_value(5.0),
			"kappa")
		("minW",po::value<double>(&mlep.minW)->default_value(1e-6),
			"min value for any element of W")
		("maxW",po::value<double>(&mlep.maxW)->default_value(10),
			"min value for any element of W")
		("minbeta",po::value<double>(&mlep.minbeta)->default_value(1e-4),
			"min value for beta")
		("nthread",po::value<int>(&mlep.nthread)->default_value(8),
			"number of threads")
		("lambda",po::value<double>(&mlep.lambda)->default_value(0.0),
			"lambda (L1-regularizing strength on Ws)")
		("lambda2",po::value<double>(&mlep.lambda2)->default_value(0.0),
			"lambda2 (L2-regularizing strength on Ws)")
		("initbeta",po::value<double>(&initbeta)->default_value(1.0/90.0),
			"starting beta value")
		("mumult",po::value<double>(&mumult)->default_value(0.1),
			"multiplier for starting mu value (multiplied by emperial rate for label)") 
		("maxinitW",po::value<double>(&mumult)->default_value(initbeta/2.0),
			"maximum initial W")
		("allinitW",po::value<int>(&allinitW)->default_value(false),
			"whether to set non-zero Ws between observed labels")
		("clampWitt",po::value<int>(&mlep.clampWitt)->default_value(0),
			"if not allinitW, number of iterations to keep clamped")
		("out",po::value<string>(&outputfilename)->default_value(string("crimedata/testout.txt")),
			"output model filename")
		("out_s",po::value<string>(&outputfilename_s)->default_value(string("crimedata/testout_short.txt")),
                        "output model filename for short data")
		("in",po::value<string>(&inputfilename)->default_value(string(" ")),
			"input model filename")
		("config",po::value<string>()->default_value(string("")),"configuration filename")
		("initseed",po::value<unsigned int>(&initseed)->default_value(0),"initial seed (0=use random device)")
		("berno_rt",po::value<double>(&berno_rt)->default_value(1.0),"bernoulli dist rate")
		("keep",po::value<int>(&keep)->default_value(0),"bias sampling rate")
		("top",po::value<int>(&top)->default_value(0),"keep top x normal location");

	po::variables_map omap;
	po::store(po::parse_command_line(argc,argv,odesc),omap);
	string cfname = omap["config"].as<string>();
	if (cfname!="") {
		cout << "parsing " << cfname << endl;
		po::store(po::parse_config_file<char>(cfname.c_str(),odesc),omap);
	}
	po::notify(omap);
	if (omap.count("help")) {
		cout << odesc << endl;
		return 1;
	}
	if (omap.count("show")) {
		cout << "nhidden=" << extralabels << endl
			<< "nem=" << mlep.nsteps << endl
			<< "maxT=" << maxT << endl
			<< "minT=" << minT << endl
			<< "nsamp=" << mlep.nsamp << endl
			<< "nburnin0=" << mlep.nburnin0 << endl
			<< "nburnin=" << mlep.nburnin << endl
			<< "nskip=" << mlep.nskip << endl
			<< "kappa=" << mlep.kappa << endl
			<< "minW=" << mlep.minW << endl
			<< "maxW=" << mlep.maxW << endl
			<< "minbeta=" << mlep.minbeta << endl
			<< "nthread=" << mlep.nthread << endl
			<< "lambda =" << mlep.lambda << endl
			<< "initbeta=" << initbeta << endl
			<< "mumult=" << mumult << endl
			<< "maxinitW=" << maxinitW << endl
			<< "allinitW=" << allinitW << endl
			<< "clampWitt=" << mlep.clampWitt << endl
			<< "out=" << outputfilename << endl
			<< "config=" << cfname << endl;
		return 1;
	}
	std::cout<< "lambda: " << mlep.lambda << std::endl;
	std::cout<< "lambda2: " << mlep.lambda2 << std::endl
			<< "minT: "<<minT <<"   maxT: "<<maxT<< std::endl;

	std::cout<< "nsamp: " <<mlep.nsamp<<" nburnin0: "<<mlep.nburnin0<< " nskip: " <<mlep.nskip  << std::endl;
	std::cout<< "kappa: " << mlep.kappa << std::endl;
	std::cout<< "minW: " << mlep.minW << " maxinitW: "<< maxinitW << std::endl;
	std::cout<< "minbeta: " << mlep.minbeta<<" initbeta: " << initbeta << std::endl;
	std::cout<< "mumult: " << mumult<< " allinitW: " << allinitW << std::endl;
	std::cout<< "input: "<<inputfilename<<std::endl;



	int nlabels = 106;
	std::vector<traj> data;

	ifstream cdata("Houston_Hour_ID.txt");
//	ifstream cdata("Houston_Minutes_ID.txt");
	if (!cdata.good()) {
		cout << "could not open inputfile " << endl;
		return 1;
	}

	data.push_back(traj{});
	double T;
	cdata >> T;

	T = std::min(T,maxT);
	T -= minT;
	std::cout << "T-=minT " << T << std::endl;
	data.back().tend = T;
	data.back().events.resize(nlabels);
	data.back().unobs.resize(nlabels);


	vector<int> nevents(nlabels,0);
	int nev = 0;
	int cnt = 0;
	vector<double> t0(nlabels,0);

//	std::set<int> keep_40 {27,50,82};
	std::set<int> keep_40 {27};
	std::set<int> keep_60 {27,50};
	std::set<int> keep_80 {15,27,50,82};
	std::set<int> keep_top {27,50,82,15,31,6,45,5,83,13};
	std::set<int> keep_topx(keep_top.cbegin(), std::next(keep_top.cbegin(), top));
	
	if (keep ==40) {
//		berno_rt = 0.086;
		berno_rt = 0.26;
	}
	else if (keep == 60) {
		berno_rt = 0.453;
	}
	else if (keep == 80) {
		berno_rt = 0.661;
	};

	// if (top == 3){
	// 	keep_top={27,50,82};
	// }
	// else if (top == 5){
	// 	keep_top={27,50,82,15,31};
	// }
	// else if (top == 8){
	// 	keep_top={27,50,82,15,31,6,45,5};
	// }
	// else if (top == 10){
	// 	keep_top={27,50,82,15,31,6,45,5,83,13};
	// };

	std::cout<<"keep= "<< keep <<  ", bernoulli rate: "<<berno_rt<<std::endl<<
				"keep top "<< top << "normal locations " <<std::endl;

	std::default_random_engine generator;
	std::bernoulli_distribution distribution(berno_rt);

	while(true) {
		int l;
		double t;
		cdata >> l >> t;
		t -= minT;
		if (cdata.eof()|| t>T ) break;
		if (t<0.0 ) continue;
		int pick = distribution(generator);
		if (keep==40 && keep_40.find(l) != keep_40.end()) pick = 1;
		if (keep==60 && keep_60.find(l) != keep_60.end()) pick = 1;
		if (keep == 80 && keep_80.find(l) != keep_80.end()) pick = 1;
		if (keep == 0 && keep_topx.find(l) != keep_topx.end()) pick = 1;
		std::cout<< "random number: "<< pick<< " label: " << l <<" t: " <<t<<std::endl;
		if (pick) {
			data.back().events[l].insert(t);
			nevents[l]++;
			nev++;
		}
		cnt++;
	}

	std::cout << "read " << nev << " events" << std::endl;
	std::cout <<"pick with rate "<<double(nev)/cnt<<" from "<<cnt<<" events "<<std::endl;
	std::cout << "unobs window empty? "<<data.back().unobs[1].empty()<<std::endl;
	// for(auto i = data.back().events[7].begin();i!=data.back().events[7].end();)
	// 	if (*i < 20 && *i>=5)
	// 		i= data.back().events[7].erase(i);
	// 	else ++i;

//	ifstream ndata("icpsr_neighbor.txt");
//	ifstream ndata("icpsr_sim.txt");
//	ifstream ndata("Houston_self.txt");
	ifstream ndata("Houston_Neighbor_race.txt");
//	ifstream ndata("Houston_Neighbor_NewFeature.txt");
	if (!ndata.good()) {
		cout << "could not open neighbor file" << endl;
		return 1;
	}
	std::vector<std::vector<int>> Neighbors; 


	std::string line;

	while(std::getline(ndata,line)) {

		std::vector<int> tokens;
		std::istringstream ls(line);
		int token;
	
		while (ls >> token)
			tokens.push_back(token);

			
		auto l = tokens[0];

		if(tokens.size())
			
//			std::cout<<tokens[0]<<" l "<<l<<std::endl;
			tokens.erase(tokens.begin());
			Neighbors.emplace_back(std::move(tokens));
	}
	std::cout<<"Neighbors size: "<<Neighbors.size()<<std::endl;

	for (int i = 0 ; i< 5; i++){
		if (Neighbors[i].size()>0){
			for ( auto j: Neighbors[i])
				std::cout<<j<<" ";
			std::cout<<"\n";
		}
	}




	

	vector<double> initmu;
	int maxc = *(max_element(nevents.begin(),nevents.end()));
	std::cout<<"nevents: "<<*nevents.begin()<<" , "<<*nevents.end()<<" maxc "<< maxc<<std::endl;
	for(auto &c : nevents) initmu.push_back(c/T * mumult);


	std::random_device rd;
	if (initseed == 0) initseed = rd();
	std::mt19937 rand(initseed);
	// std::uniform_real_distribution<> wdist(0.0,maxinitW);
	std::cout<<"maxinitW: "<<maxinitW<<std::endl;
	std::uniform_real_distribution<> wdist(0.0,mlep.minW);


	vector<pair<double,double>> noobs(1,pair<double,double>{0.0,T});
	for(int i=0;i<extralabels;i++) {
		initmu.push_back(maxc/T * 0.1);
		data.back().events.emplace_back();
		data.back().unobs.emplace_back(noobs);
	}

	vector<vector<double>> initW(nlabels+extralabels,vector<double>(nlabels+extralabels,0.0));
	for(int i=0;i<nlabels+extralabels;i++) 
		for(int j=0;j<nlabels+extralabels;j++)
			initW[i][j] = !allinitW && i<nlabels && j<nlabels ? 0 : wdist(rand);
	std::cout<< "W= "<<initW.size()<<" , "<<initW[0][0]<<std::endl;

	hp<multikernel<singleexpkernel>> process(initmu,initW,1,initbeta);



	ofstream ofs;
	if (outputfilename!="") {
		ofs.open(outputfilename.c_str());
		if (!ofs.good()) {
			cout << "could not open " << outputfilename << " for writing" << endl;
			return 2;
		}
	}

	mle(process,data,rand,mlep,Neighbors);

	if (outputfilename!="") {
		ofs << "configfile = " << cfname << endl;
		ofs << "initseed = " << initseed << endl;
		process.kernel.save(ofs);
	}
}


