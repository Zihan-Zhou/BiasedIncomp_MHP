#include "mle_orig.hpp"
#include "kernels.hpp"
#include "hp.hpp"
#include <random>
#include <limits>
#include <iostream>

template<typename K, typename R>
void testpred(const hp<multikernel<K>> &p, traj data,
		std::map<double,std::vector<int>> toadd,const mleparams &params,
		int nobs, std::ostream &os,std::ostream &osID, R &rand) {

	data.unobs.resize(p.kernel.baserates.size());
	data.events.resize(data.unobs.size());

	std::vector<R> rs;
	for(int i=0;i<params.nthread;i++) rs.emplace_back(rand());

	std::cout << "looping for " << toadd.size() << " new events" << std::endl;
	int all_tot = 0;
	int all_right = 0;
	for(auto &next : toadd) {

		double t0 = next.first;
		double wint = data.tend+(t0-data.tend+1)*1;
		std::cout << "predicting from " << data.tend << " to " << t0 << " for event of label " ;
		for (auto &li : toadd[t0] ) std::cout<< li << " ";
		std::cout<< " with window until " << wint << std::endl;
		for(int i=0;i<nobs;i++) {
			data.unobs[i].clear();
			data.unobs[i].emplace_back(data.tend,wint);
//			std::cout<<"first nobs: " <<i<<", " << nobs<<std::endl;
		}
//		std::cout<<"first nobs: " <<i<<", " << nobs<<std::endl;
		for(int i=nobs;i<p.kernel.baserates.size();i++) {
			data.unobs[i].clear();
			data.unobs[i].emplace_back(0,wint);
//			std::cout<<"2nd loop happen, i: "<<i<<std::endl;
		}
//		std::cout<< " size: "<<p.kernel.baserates.size()  <<std::endl;
		data.tend = wint;

		std::vector<typename hp<multikernel<K>>::gibbsstate> states;
		for(int i=0;i<params.nthread;i++)
			states.emplace_back(p.initgibbs(data,params.kappa,rand));

		std::vector<std::vector<int>> ans;
		for(int i=0;i<params.nthread;i++)
			ans.emplace_back(nobs,0);

		std::vector<std::future<void>> futs(params.nthread);
		for(int i=0;i<params.nthread;i++) {
			auto &rr = rs[i];
			auto &si = states[i];
			auto &a = ans[i];
			int ns = params.nsamp*(i+1)/params.nthread
					- params.nsamp*i/params.nthread;
			int nburn = params.nburnin0;
			futs[i] = std::async(std::launch::async,
					[&rr,&si,&p,&a,t0,nburn,ns]() {
						pred1(p,si,a,t0,nburn,ns,rr);
						});
		}
		for(auto &f : futs)
			f.wait();

//		os << t0 << ' ' << next.second;
		os << t0 << ' ';
		osID << t0 << ' ';
		for (auto &li : toadd[t0]){
			os <<  li ;
			osID<< li<<' ';
		}

		int tot = 0;
		int all_sampNZ = 0;
		std::vector<int> all_samped(nobs,0);
		for(int i=0;i<nobs;i++){
			for(auto &v : ans) {
				tot += v[i];				
				if (v[i]>0) all_samped[i] =1;
//				if (v[i]>0) std::cout<< " v["<<i<<"]: "<<v[i]<<"      allsampled["<<i<<"] "<<all_samped[i] <<std::endl;
			}
			all_sampNZ += all_samped[i];

		}
//		std::cout<<" all_sampNZ "<<all_sampNZ<<std::endl;

		all_tot += tot;
		int sright = 0;
		int True_Pos = 0;
		int False_Pos = 0;
		int True_Neg = 0;
		double prec = 0.0;
		double recall = 0.0;
		double F_1 = 0.0;
//		for(auto &v : ans) sright += v[next.second];
		for(auto &v : ans) {

			for (auto &li : toadd[t0]) {
				sright += v[li];
//				std::cout<< " v["<<li<<"]: "<<v[li]<<std::endl;
			}

		}
		for (auto &li : toadd[t0]) {
//				std::cout<< " all_samped["<<li<<"]: "<<all_samped[li]<<std::endl;
				if (all_samped[li]>0) True_Pos +=1;
				if (all_samped[li]==0) True_Neg +=1;			
		}
		all_right += sright;
		False_Pos = all_sampNZ - True_Pos;
		prec = (double)True_Pos/(True_Pos+False_Pos);
		recall = (double)True_Pos/(True_Pos+True_Neg);
		F_1 = (double)2*(prec*recall)/(prec+recall);

		os << ' ' << (double)sright/tot;

		os << ' ' << sright << ' ' << tot;
		os << ' ' << True_Pos << ' ' << False_Pos<< ' ' << True_Neg<<' '<<all_sampNZ;
		os << ' ' << prec << ' ' << recall << ' '<<F_1;
		std::cout<<" tot: " << tot <<" rate: "<<(double)sright/tot<<std::endl; 
		std::cout<< "True_Pos: "<<True_Pos<< " False_Pos: "<<False_Pos<<" True_Neg: "<<True_Neg
				<<" all_samped: " << all_sampNZ
				<<" Prec: " << (double)True_Pos/(True_Pos+False_Pos)<<' '
				<<" Recall: "<< (double)True_Pos/(True_Pos+True_Neg)<<' ' 
				<<" F-1: " <<F_1 <<std::endl; 
		for(int i=0;i<nobs;i++) {
			int s = 0;
			for(auto &v : ans) s += v[i];
			os << ' ' << s;
		}
		os << std::endl;
		osID << std::endl;

//		data.events[next.second].emplace(t0);
		for (auto &li : toadd[t0]){
			data.events[li].emplace(t0);
		}
		data.tend = next.first;
	}
	os << std::endl << (double)all_right/all_tot;
	std::cout<<std::endl<<"Final Accuracy: "<<(double)all_right/all_tot;
}

template<typename K, typename R>
void pred1(const hp<multikernel<K>> &p,
		typename hp<multikernel<K>>::gibbsstate &state,
		std::vector<int> &ansdist, double t0, 
		int nburn, int nsamp, R &rand) {
	for(int i=0;i<nburn;i++)
		p.gibbsstep(state,rand);
	for(int i=0;i<nsamp;i++) {
		p.gibbsstep(state,rand);
		int lnext=-1;
		double lt = std::numeric_limits<double>::infinity();
		for(int l=0;l<ansdist.size();l++) {
			auto lb = state.curr.events[l].upper_bound(t0);
			if (lb != state.curr.events[l].end() 
					&& *lb<lt) {
				lnext = l;
				lt = *lb;
			}
		}
		if (lnext!=-1) ansdist[lnext]++;
	}
}
