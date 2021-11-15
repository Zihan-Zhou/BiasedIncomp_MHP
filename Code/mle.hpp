
#ifndef MLE_HPP
#define MLE_HPP

#include "hp.hpp"
#include "kernels.hpp"
#include <vector>
#include <cmath>
#include <iostream>
#include <future>
#include <tuple>
#include <string.h>
using namespace std::chrono;

struct mless {

	mless(int nlabels) : nl(nlabels), Nplus(0),
		Nl(nlabels,0), Nzero(nlabels,0),
		N(nlabels,std::vector<int>(nlabels,0)),
		R(0), TT(0), Ti(nlabels,std::vector<std::tuple<double,double,int,double>>(0)),
		Events(nlabels,std::vector<double>(0)),
		Vij(nlabels,std::vector<double>(nlabels,0)),
		Cij(nlabels,std::vector<double>(nlabels,0)),Ntotal(0),Nnormal(0) {}
		//Ti(nlabels,std::vector<std::pair<double,double>>(0)) {}
	std::size_t nl;
	// total number of events that have non-root parents
	int Nplus;

	// total number of events spawned from each label
	std::vector<int> Nl;
	// total number of events that have root parents, for each label
	std::vector<int> Nzero;
	// N[i][j] is number of events of label j spawned from label i
	std::vector<std::vector<int>> N;
	// sum of times from event to child
	double R;
	// total amount of time
	double TT;
	// for each label, for each event, T - t_i and t_{par} - t_i 
	//  (last is negative if no parent)
	std::vector<std::vector<std::tuple<double,double,int,double>>> Ti;
	// for each label, for each event, store Li[l]=lai (parent)
	//std::vector<std::vector<int>> Li;
	//std::vector<int> Li;
	//tend
	double T;
	std::vector<std::vector<double>> Events;
	std::vector<std::vector<double>> Vij;
	std::vector<std::vector<double>> Cij;
	int Ntotal;
	int Nnormal;

//	std::vector<std::vector<double>> P;

	
	template<typename K>
	void Pmiss(int nsamp, const hp<multikernel<K>> &p, const typename hp<multikernel<K>>::gibbsstate &samp,
		const std::vector<traj> &data){
		using etype= typename hp<multikernel<K>>::gibbsstate::etype;

		for(auto &e : samp.events) {

			if (e.second.e==etype::root || e.second.e==etype::virt) 
				continue;
			if (e.second.par->second.e!=etype::root) Ntotal++;
//			if (e.second.par->second.e!=etype::root && e.second.e!=etype::evid) Nnormal++;
			double t = e.first.t;
			int lai = e.first.label;//event label as parent
			
			

			for (auto &x:data) {
				for (int j = 0; j<x.unobs[lai].size(); j++){
						if (t<=x.unobs[lai][j].second && t>x.unobs[lai][j].first && e.second.e!=etype::evid) Cij[lai][j]+=1;
						
					}


			}
			// if (e.second.e!=etype::evid) std::cout<< "Pmiss: label"<< e.first.label << " parent: "<<e.second.par->first.label
			// << " time: "<<e.first.t << " type: "<< samp.type2str(e.second.e) <<std::endl;

			if(!std::count(Events[lai].begin(),Events[lai].end(),t)){
				if (e.second.par->second.e!=etype::root && e.second.e!=etype::evid) Nnormal++;
				
				for(int l = 1; l<nl;l++) {				

					for (auto &x:data) {						
						for (int j = 0; j<x.unobs[l].size(); j++){
							if (t<x.unobs[l][j].second){	
							Vij[l][j] -= (p.kernel.intphi(lai,l,0,x.unobs[l][j].second-t)
								-p.kernel.intphi(lai,l,0,x.unobs[l][j].first-t));
							}
							else if (t<x.unobs[l][j].second){	
							Vij[l][j] -= (p.kernel.intphi(lai,l,0,x.unobs[l][j].second-t)
								-p.kernel.intphi(lai,l,0,x.unobs[l][j].first-t));
							}

							else if (j==x.unobs[l].size()) Vij[l][j] -= (p.kernel.intphi(lai,l,0,T-t)
								- p.kernel.intphi(lai,l,0,x.unobs[l][j-1].second-t));
	
						}
					}
				}
				Events[e.first.label].emplace_back(e.first.t);
			}

		}
	};
		

	template<typename K>
	void addstate(const hp<multikernel<K>> &p, const typename hp<multikernel<K>>::gibbsstate &samp,
		const std::vector<traj> &data) {
		using etype= typename hp<multikernel<K>>::gibbsstate::etype;
		double T = samp.curr.tend;
		TT += T;
		double pm = 0.0;
		double cm = 0.0;
//		std::cout<<"add states "<<std::endl;


		for(auto &e : samp.events) {
			if (e.second.e==etype::root || e.second.e==etype::virt) 
				continue;
			// if(e.second.e!=etype::evid) std::cout<< "label"<< e.first.label << " parent: "<<e.second.par->first.label
			// << " time: "<<e.first.t << " type: "<< samp.type2str(e.second.e) <<std::endl;
			if (e.second.par->second.e==etype::root) {
				Nzero[e.first.label]++;
			} else {
				R += e.first.t - e.second.par->first.t;
				N[e.second.par->first.label][e.first.label]++;
				Nl[e.second.par->first.label]++;
				Nplus++;
			}




			int i = e.first.label;
			int j = 0;	
			int indx = 0; 
			double t = e.first.t;
			for (auto &x:data) {				
				while(j<x.unobs[i].size() && t>x.unobs[i][j].second) ++j;
				if (j==x.unobs[i].size()) {
					pm=1-std::exp(Vij[i][j]);
	//				cm = Cij[i][-1]/(Ntotal*(T-x.unobs[i][-1].second));
					cm = Cij[i][j]/Nnormal;
					indx = j;
				}
				else if (t>x.unobs[i][j].first) {
					pm=1-std::exp(Vij[i][j]);
	//				cm = Cij[i][j]/(Ntotal*(x.unobs[i][j].second-x.unobs[i][j].first));
					cm = Cij[i][j]/Nnormal;
					indx = j;
				}
				else {
					pm = 1-std::exp(Vij[i][j-1]);
	//				cm = Cij[i][j-1]/(Ntotal*(x.unobs[i][j-1].second-x.unobs[i][j-1].first));
					cm = Cij[i][j-1]/Nnormal;
					indx = j-1;
				}

				
			}
			// std::cout<< "addstate indx "<<indx<<std::endl;

		
			Ti[e.first.label].emplace_back(std::make_tuple(T-e.first.t,
								e.second.par->second.e!=etype::root
								? e.first.t-e.second.par->first.t
								: -1.0,e.second.par->first.label,
								e.second.e==etype::norm?indx:99));


		}

	}
};

// doubling to bracket, following by golden section line search
template<typename F>
double fnmin(F &f, double x, double res=1e-3) {
	double x0 = res, x1 = x, x2 = x*2;
	double f0 = f(x0), f1 = f(x1), f2 = f(x2);
	while(f0<f1 || f1>f2) {
		x0 = x1; f0 = f1;
		x1 = x2; f1 = f2;
		x2 = x2*2; f2 = f(x2);
	}
	constexpr double ratio=0.618;
	double xb = x0 + (1.0-ratio)*(x2-x0);
	double xc = x0 + ratio*(x2-x0);
	double fb = f(xb);
	double fc = f(xc);
	while(x2-x0 > x2*res) {
		if (fb>fc) {
			x0 = xb; f0 = fb;
			xb = xc; fb = fc;
			xc = x0 + ratio*(x2-x0);
			fc = f(xc);
		} else {
			x2 = xc; f2 = fc;
			xc = xb; fc = fb;
			xb = x0 + (1.0-ratio)*(x2-x0);
			fb = f(xb);
		}
	}
	return x0;
}


// for the moment, only for 1-parameter base kernel!!
template<typename K>
void mleopt(multikernel<K> &k, const std::vector<mless> &ss,
		int nitt=1, double minW =1e-3, double maxW = 1e3, double minbeta=1e-3,
		double lambda=0.0, bool clamp=false) {




	k.skernel.setparam(0,1.0);

	auto oneval = [&ss,&k](int i, int l) {
		double sum = 0.0;
		double ret = 0.0;
		double log_Na = 0.0;
		double log_N0 = 0.0;
		double log_TT = 0.0;
		double log_sum = 0.0;


		for(auto &dt : ss[i].Ti[l]) {

			int lai = std::get<2>(dt);
			auto v = k.skernel.intphi(0.0,std::get<0>(dt));//l is parent;
			sum += v; //term 3.1.2

			if (std::get<1>(dt)>=0)			
				ret += k.skernel.logphi(std::get<1>(dt));//term 3.2
			if (lai>=0 and ss[i].N[lai][l]>0){
				log_Na += std::log(ss[i].N[lai][l]); //term3.1.1
			
			}


		}
		
		ret = ret + log_Na ;
		return std::pair<double,double>(sum,ret);
//		return std::tuple<double,double,double>(sum,ret,log_Na);
	};

	auto negllh = [&ss,&k,&oneval,&lambda](double beta) {
		double ret = 0.0;		
		k.skernel.setparam(1,beta);
		for(int l=0;l<ss[0].nl;l++) {
			int Nl = 0;	
			int N= 0;	
			int Nzero = 0;
			double TT = 0;
			double sum = 0.0;
			double mu = k.baserates[l];
			double log_mu = 0.0;
			if (mu>0) log_mu =std::log(mu);

			std::vector<std::future<std::pair<double,double>>> futs(ss.size());
			for(int i=0;i<futs.size();i++)
				futs[i] = std::async(std::launch::async,oneval,i,l);
			for(int i=0;i<futs.size();i++) {
				auto res = futs[i].get();
				sum += res.first;
				ret += res.second;
				Nl += ss[i].Nl[l];
				if (l>0) N+=ss[i].Nl[l];		
				Nzero += ss[i].Nzero[l]; //term2;
				TT +=ss[i].TT;

			}
			

//			double log_baser = std::log(std::min(10.0,std::max(1e-6,(double)Nzero/TT))) ;

//			ret = ret +Nl - Nzero + log_baser;

//			std::cout<< "l= "<< l << " , N= "<< N <<std::endl;
//			ret = ret -Nzero +log_baser ;
			ret += Nl;
			if (Nzero>0) ret = ret-mu*TT+ log_mu;//log_baser;

			if (Nl>0)
				ret -= Nl*std::log(lambda+sum);
				
		}
//		std::cout<< "ret= "<< ret<< std::endl;

		return -ret;
	};

	


	k.skernel.setparam(1,fnmin(negllh,k.skernel.getparam(1),minbeta));

	std::cout << "W = " << std::endl;
	for(int l=0;l<ss[0].nl;l++) {
		double den = lambda;
		for(auto &ssi : ss)
			for(auto &dt : ssi.Ti[l])
			{	
				den += k.skernel.intphi(0.0,std::get<0>(dt));

			}
		for(int lp=0;lp<ss[0].nl;lp++) {
			int Nllp = 0;
			for(auto &ssi : ss)
				Nllp += ssi.N[l][lp];
			if (clamp && k.W[l][lp]==0)
				k.W[l][lp] = Nllp/den;
			else
				k.W[l][lp] = std::min(maxW,std::max(minW,Nllp/den));
			if (k.W[l][lp]>minW)
				std::cout << '(' << l << ',' << lp << ") = " << k.W[l][lp] << std::endl;
		

		}

	}
	

	std::cout << "mu = " << std::endl;
	for(int l=0;l<ss[0].nl;l++) {
		int TT = 0;
		int Nzero = 0;
		for(auto &ssi : ss) {
			TT += ssi.TT;
			Nzero += ssi.Nzero[l];
		}

		k.baserates[l] = std::min(maxW,std::max(minW,(double)Nzero/TT));
		//std::cout<< "Nzero= "<< Nzero << "TT= "<< TT <<"; ";
		std::cout << k.baserates[l] << ' ';
	}
	std::cout << std::endl;

	std::cout << "end beta = " << k.skernel.beta << std::endl;
	//std::cout << " negllh = " << negllh(k.skernel.beta) <<std::endl;
	k.setWstats();
	std::cout << " negllh = " << negllh(k.skernel.beta) <<std::endl;
}

	



template<typename K>
void mleopt_sgd(multikernel<K> &k, const std::vector<mless> &ss, const std::vector<std::vector<int>> &Neighbors,
		int nitt=1, double minW =1e-3, double maxW = 1e3, double minbeta=1e-3,
		double lambda=0.0, double lambda2=0.0,bool clamp=false) {

	k.skernel.setparam(0,1.0);
//	std::cout<<"mleopt_sgd: Neighbors"<<Neighbors.size()<<std::endl;

	auto oneval_sgd = [&ss,&k](int i, int l) {
		double sum = 0.0;
		double ret = 0.0;
		double log_mu = 0.0;
		double sum_pmiss = 0.0;
//		std::cout<<"oneval_sgd "<<std::endl;
		for(auto &dt : ss[i].Ti[l]) {
			int lai = std::get<2>(dt);
			int j = std::get<3>(dt);
			double pmiss = 1.0;
			// std::cout<<"mleopt_sgd j: "<<j<<std::endl;


			if (j!=99) {
				pmiss = 1-exp(ss[i].Vij[l][j]);
				// pmiss = ss[i].Cij[l][j]/ss[i].Nnormal;
			}


			auto v = k.intphi(l,0.0,std::get<0>(dt));//l is parent; sum over child;
			sum += v; //term 1

			if (std::get<1>(dt)>=0)		
				ret += std::log(k.phi(lai,l,std::get<1>(dt)));
			
			if (pmiss>0) sum_pmiss += std::log(pmiss);
				
			}

		return std::pair<double,double>(sum,ret+sum_pmiss);
	};

	auto regu2 = [&k,&Neighbors](int l){
		double ret=0;
		double Wsum_l = 0;


		for (int i = 0; i<k.W.size(); i++){
			Wsum_l += k.W[i][l];

		}
//		std::cout<<std::endl<<"l= "<<l << " Wsum_l= "<< Wsum_l<< std::endl;
		if (Neighbors[l].size()>0){
//loop over all neighbors of l
			for (int i = 0; i<Neighbors[l].size(); i++){
				double Wsum_neb = 0;
				int neb=Neighbors[l][i]-1;

				// std::cout<<l<< "'s Neighbors: "<< neb<<std::endl;
	//calculate W.neb = sum(W[j][neb])=sum(Wtrans[neb][j])
				for (int j = 0; j<k.W.size(); j++){
					
					Wsum_neb += k.W[j][neb];
				}
//				std::cout<< "Wsum_neb= "<< Wsum_neb<< std::endl;
				ret += pow((k.Wsum[l]-k.Wsum[neb]) , 2.0)+ pow((Wsum_l-Wsum_neb),2.0);
//				std::cout<<"label: "<<l << " Neighbor: "<<neb << " ret: "<< ret<< std::endl;
			}
		}
//		std::cout<<"label: "<<l << " ret: "<< ret<< std::endl;
		return ret;

	};


	
	auto negllh_sgd = [&ss,&k,&oneval_sgd,&lambda,&lambda2,&regu2](double beta) {
		double ret = 0.0;
		k.skernel.setparam(1,beta);
		
		for(int l=0;l<ss[0].nl;l++) {


			int Nl = 0;		
			int Nzero = 0;
			
			double sum = 0.0;
			double TT = 0.0;
			double log_mu =0.0;
			double sum_W = 0.0;
			double mu = k.baserates[l]; 
			double reg2=0.0;
			// std::cout<<"negllh_sgd: reg2( "<<l<<" )=  ";
			reg2=regu2(l);
//			std::cout<<reg2<<std::endl;

			sum_W += k.Wsum[l];
			if (mu>0) log_mu =std::log(mu);

			std::vector<std::future<std::pair<double,double>>> futs(ss.size());
			for(int i=0;i<futs.size();i++)
				futs[i] = std::async(std::launch::async,oneval_sgd,i,l);
			for(int i=0;i<futs.size();i++) {
				auto res = futs[i].get();
				sum += res.first;
				ret += res.second;
				Nl += ss[i].Nl[l]; 
				Nzero += ss[i].Nzero[l]; //term2;
				TT += ss[i].TT;
					
			}
//			std::cout<< "negllh_sgd: sum-Nl= "<< sum << " , "<< Nl<< " , "<< sum-Nl<<std::endl;
			if (Nzero>0) ret = ret-mu*TT+log_mu;
			ret = - sum + ret -lambda*sum_W -lambda2*reg2;
			// std::cout<<"negllh_sgd: Nzero "<<Nzero<<
			// 			" ,-sum "<<-sum<<
			// 			" ,ret " << ret<<
			// 			" ,reg1: "<< -lambda*sum_W<< 
			// 			" ,reg2 "<< lambda2*reg2<<std::endl;
			

		}
		// std::cout<<"negllh_sgd: -ret= "<<-ret<<std::endl;
		return -ret;
	};

	//SGD
//	double rate = 1e-13; //learn rate
	double rate = 1e-13;
	k.skernel.setparam(1,fnmin(negllh_sgd,k.skernel.getparam(1),minbeta));
//	k.skernel.setparam(1,0.0110671);
	double diff_negllh = -1;
	for (int i = 0; i<100; i++){
//		std::cout<<" SGD i = "<< i<<std::endl;
//	double relative_W_diff= 1e10;

	// int i = 0;
	// double bf_den = 0.0;

	// while(std::abs(diff_negllh)> 100){
		
		double negllh_i = negllh_sgd(k.skernel.beta);
//		std::cout<< " SDG negllh_i = "<<negllh_i<<" beta= "<<k.skernel.beta<<std::endl;
		if (std::abs(diff_negllh)<100) rate/=10;

//		std::cout<<std::endl<<"learning rate:  "<< rate << std::endl;

		auto start = high_resolution_clock::now(); 
//		k.skernel.setparam(1,fnmin(negllh_sgd,k.skernel.getparam(1),minbeta));
	
		double total_w = 0.0;
		double total_W_diff =0.0;
		double relative_W_diff = 0.0;	


		std::cout << "W = " << std::endl;
		for(int l=0;l<ss[0].nl;l++) {


			double den = lambda;
			double diff_W =0.0;
//			std::cout<<" SGD l= "<<l;
			for(auto &ssi : ss)
				for(auto &dt : ssi.Ti[l])
				{	
					den += k.skernel.intphi(0.0,std::get<0>(dt));
				}


			for(int lp=0;lp<ss[0].nl;lp++) {


				int Nllp = 0;
				int Nb_l = 0;
				int Nb_lp = 0;
				double R2 = 0.0;


				Nb_l = Neighbors[l].size();
				Nb_lp = Neighbors[lp].size();
				R2 = (Nb_l+Nb_lp)*k.W[l][lp];

//				std::cout<< "SGD lp: l= "<<l<< " , lp= "<<lp<<" , R2= "<<R2<<std::endl;

				if (Nb_l>0){
					for (auto i : Neighbors[l]){
//						std::cout<< " Nb_l "<< i-1<<std::endl;
						R2-=k.W[i][lp];
					}
				}
				if (Nb_lp>0){
					for (auto i : Neighbors[lp]){
//						std::cout<< " Nb_lp "<< i-1<<std::endl;
						R2-=k.W[l][i];
					}
				}
//				if (R2!=0) std::cout<<"R2: "<<R2<<std::endl;

				for(auto &ssi : ss)
					Nllp += ssi.N[l][lp];
				if ((Nllp <=den*minW )|| (k.W[l][lp]==0 ))
					{k.W[l][lp] = minW;
					
				}
				else
//					std::cout<<"R2: "<< R2<<std::endl;
				//	k.W[l][lp] = std::min(maxW,std::max(minW,Nllp/den));

					k.W[l][lp] -= rate*(den - Nllp/k.W[l][lp]-2*lambda2*R2);
					k.W[l][lp] = std::min(maxW,std::max(minW,k.W[l][lp]));
				if (k.W[l][lp]>minW)  std::cout << '(' << l << ',' << lp << ") = " << k.W[l][lp] << std::endl;
	
				double mle_W = std::min(maxW,std::max(minW,Nllp/den));

				diff_W = std::abs(k.W[l][lp]- mle_W);
				relative_W_diff += diff_W/mle_W;
				total_w += std::min(maxW,std::max(minW,Nllp/den));
				total_W_diff+=diff_W;


			}	


		}
	
//		std::cout <<std::endl<< "relative_W_diff = " << relative_W_diff << "  "
//		<< "total_W_diff = " << total_W_diff << std::endl;
		

		std::cout << "mu = " << std::endl;
		double diff_mu = 0.0;
		double total_mu = 0.0;
		double relative_mu_diff = 0.0;
		for(int l=0;l<ss[0].nl;l++) {
			int TT = 0;
			int Nzero = 0;
			
			for(auto &ssi : ss) {
				TT += ssi.TT;
				Nzero += ssi.Nzero[l];
			}

			if (k.baserates[l]==0)
				k.baserates[l] == minW;
			else
				k.baserates[l] -= rate*(TT-Nzero/k.baserates[l]);//-slope of negllh;
				k.baserates[l] = std::min(maxW,std::max(minW,k.baserates[l]));
			std::cout << k.baserates[l] << ' ';
			diff_mu = std::abs(k.baserates[l]-std::min(maxW,std::max(minW,(double)Nzero/TT)));
			total_mu += diff_mu;
			relative_mu_diff += diff_mu/std::min(maxW,std::max(minW,(double)Nzero/TT));

		
		}

		k.setWstats();
		// std::cout << i<< " -th beta_sgd = " << k.skernel.beta << std::endl;
		// std::cout << i << " -th" <<  " negllh_sgd = " << negllh_sgd(k.skernel.beta) <<std::endl;

		// auto stop = high_resolution_clock::now(); 
		// auto duration = duration_cast<seconds>(stop - start); 
		// std::cout << "Time Consumed: "<< duration.count() << std::endl; 
//		std::cout<<"after learning negllh= "<<negllh_sgd(k.skernel.beta)<<" beta= "<<k.skernel.beta<<std::endl;
		diff_negllh = negllh_sgd(k.skernel.beta)-negllh_i;
//		std::cout<< i << " -th diff_negllh = "<< diff_negllh<<std::endl;
		if (diff_negllh>=0) {
			std::cout<<"break at "<<i<<std::endl;
			break;}
		// i+=1;
	}

	std::cout << std::endl;
	std::cout << "Number of Normal Events: "<< ss[0].Nnormal<<std::endl;
	std::cout << "end beta_sgd = " << k.skernel.beta << std::endl;
	//k.setWstats();
	std::cout << " negllh_sgd = " << negllh_sgd(k.skernel.beta) <<std::endl;

}







template<typename K, typename R>
void mlestep(const hp<multikernel<K>> &p,
		std::vector<typename hp<multikernel<K>>::gibbsstate> &states,
		mless &ss, int nsamp, int nburnin, int nskip,
		R &rand,const std::vector<traj> &data) {
//	std::cout << "mlestep: states " << &states << std::endl << " nsamp " << nsamp << " nburnin " << nburnin << " nskip " << nskip <<  std::endl;
	for(auto &s : states) {
		for(int i=0;i<nburnin;i++)
			p.gibbsstep(s,rand);
		for(int i=0;i<nsamp;i++) {
			p.gibbsstep(s,rand);
	//		std::cout<<"mlestep: Vij in step: "<<ss.Vij[1][1]<<std::endl;
			for(int j=0;j<nskip;j++)
				p.gibbsstep(s,rand);
	//		std::cout<< "mlestep: samp: "<< i <<std::endl;
			ss.Pmiss<K>(i,p,s,data);
			ss.addstate<K>(p,s,data);
//			std::cout<< "kernel "<< p.kernel.skernel.intphi(0,1)<<std::endl;
//			ss.Pmiss_1<K>(p.kernel,s,data);
		}
		// ss.Pmiss<K>(p,s,data);
	}
}

struct mleparams {
	double lambda = 0.0;
	double lambda2 = 0.0;

	int nsteps=10;
	int nsamp=100;
	int nburnin0=1000;
	int nburnin=0;
	int nskip=0;
	double kappa=2;
	double minW=0.001;
	double maxW=1000;
	double minbeta=0.001;
	int nthread=4;
	int clampWitt=0;
};

template<typename K,typename R>
void mle(hp<multikernel<K>> &p, const std::vector<traj> &data, R &rand,
		const mleparams &params, const std::vector<std::vector<int>> &Neighbors) {
	std::vector<std::vector<typename hp<multikernel<K>>::gibbsstate>>
				states(params.nthread);

	std::cout << "nthread " << params.nthread << std::endl;


	for(int i=0;i<params.nthread;i++)
		for(auto &x : data)
			states[i].emplace_back(p.initgibbs(x,params.kappa,rand));

	std::vector<R> rs;

	for(int i=0;i<params.nthread;i++) rs.emplace_back(rand());


	for(int step=0;step<params.nsteps;step++) {
		std::cout<<"step: "<<step<<" , base "<<p.kernel.baserates.size()<<std::endl;
		std::vector<mless> ss(params.nthread,p.kernel.baserates.size());
		std::vector<std::future<void>> futs(params.nthread);
		std::cout<<"ss "<<ss.size()<<" futs "<<futs.size()<<std::endl;

		for(int i=0;i<params.nthread;i++) {
			auto &rr = rs[i];
			auto &si = states[i];
			auto &ssi = ss[i];
			int nburn = step==0 ? params.nburnin0 : params.nburnin;
			int ns = params.nsamp*(i+1)/params.nthread - params.nsamp*i/params.nthread;

			futs[i] = std::async(std::launch::async,
					[&rr,&si,&ssi,&p,nburn,&params,ns,data]() {

						mlestep(p,si,ssi,ns,nburn,params.nskip,rr,data);
					});
		}
		for(auto &f : futs)
			f.wait();

//		mleopt(p.kernel,ss,1,params.minW,params.maxW,params.minbeta,params.lambda,
//	 		step<params.clampWitt);

		mleopt_sgd(p.kernel,ss, Neighbors,1,params.minW,params.maxW,params.minbeta,params.lambda,params.lambda2,
				step<params.clampWitt);
//		auto start = high_resolution_clock::now();  		
//		auto stop = high_resolution_clock::now(); 
//		auto duration = duration_cast<seconds>(stop - start); 
//		std::cout <<"Time for negllh "<< duration.count() << std::endl; 
		std::cout << "end iteration " << step << std::endl;

		

	}
	// mleopt_sgd(p.kernel,ss_sgd,1,params.minW,params.maxW,params.minbeta,params.lambda,
	// 	false);


}

#endif
