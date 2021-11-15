
########### Original AAAI hidden 5 nodes rebalance ################
./runcrime --config=cfg-Chicago-hidden5.txt --top=1 --berno_rt=0.2 --out=RebalData/Chicago_hd5bs20Top1_Rebal_orig.model
./runcrime --config=cfg-Chicago-hidden5.txt --top=1 --berno_rt=0.4 --out=RebalData/Chicago_hd5bs40Top1_Rebal_orig.model
./runcrime --config=cfg-Chicago-hidden5.txt --top=1 --berno_rt=0.6 --out=RebalData/Chicago_hd5bs60Top1_Rebal_orig.model
./runcrime --config=cfg-Chicago-hidden5.txt --top=2 --berno_rt=0.6 --out=RebalData/Chicago_hd5bs60Top2_Rebal_orig.model
./runcrime --config=cfg-Chicago-hidden5.txt --top=3 --berno_rt=0.6 --out=RebalData/Chicago_hd5bs60Top3_Rebal_orig.model
./runcrime --config=cfg-Chicago-hidden5.txt --top=5 --berno_rt=0.6 --out=RebalData/Chicago_hd5bs60Top5_Rebal_orig.model

./tstcrime --in=RebalData/Chicago_hd5bs20Top1_Rebal_orig.model --out=RebalData/Chicago_hd5bs20Top1_Rebal_orig.txt
./tstcrime --in=RebalData/Chicago_hd5bs40Top1_Rebal_orig.model --out=RebalData/Chicago_hd5bs40Top1_Rebal_orig.txt
./tstcrime --in=RebalData/Chicago_hd5bs60Top1_Rebal_orig.model --out=RebalData/Chicago_hd5bs60Top1_Rebal_orig.txt
./tstcrime --in=RebalData/Chicago_hd5bs60Top2_Rebal_orig.model --out=RebalData/Chicago_hd5bs60Top2_Rebal_orig.txt
./tstcrime --in=RebalData/Chicago_hd5bs60Top3_Rebal_orig.model --out=RebalData/Chicago_hd5bs60Top3_Rebal_orig.txt
./tstcrime --in=RebalData/Chicago_hd5bs60Top5_Rebal_orig.model --out=RebalData/Chicago_hd5bs60Top5_Rebal_orig.txt




########### Our Methods: Missing Window with Probability #############

## biased sample
./runcrime --config=cfg-Chicago-hidden0.txt --lambda=150000 --lambda2=20000 --top=1 --berno_rt=0.2 --out=RebalData/Chicago_bs20Top1_Rebal_sgd.model
./runcrime --config=cfg-Chicago-hidden0.txt --lambda=100000 --lambda2=20000 --top=1 --berno_rt=0.4 --out=RebalData/Chicago_bs40Top1_Rebal_sgd.model
./runcrime --config=cfg-Chicago-hidden0.txt --lambda=150000 --lambda2=25000 --top=1 --berno_rt=0.6 --out=RebalData/Chicago_bs60Top1_Rebal_sgd.model
./runcrime --config=cfg-Chicago-hidden0.txt --lambda=150000 --lambda2=25000 --top=2 --berno_rt=0.6 --out=RebalData/Chicago_bs60Top2_Rebal_sgd.model
./runcrime --config=cfg-Chicago-hidden0.txt --lambda=150000 --lambda2=20000 --top=3 --berno_rt=0.6 --out=RebalData/Chicago_bs60Top3_Rebal_sgd.model
./runcrime --config=cfg-Chicago-hidden0.txt --lambda=150000 --lambda2=20000 --top=5 --berno_rt=0.6 --out=RebalData/Chicago_bs60Top5_Rebal_sgd.model

./tstcrime --in=RebalData/Chicago_bs20Top1_Rebal_sgd.model --out=RebalData/Chicago_bs20Top1Rebal_sgd.txt
./tstcrime --in=RebalData/Chicago_bs40Top1_Rebal_sgd.model --out=RebalData/Chicago_bs40Top1Rebal_sgd.txt
./tstcrime --in=RebalData/Chicago_bs60Top1_Rebal_sgd.model --out=RebalData/Chicago_bs60Top1Rebal_sgd.txt
./tstcrime --in=RebalData/Chicago_bs60Top2_Rebal_sgd.model --out=RebalData/Chicago_bs60Top2Rebal_sgd.txt
./tstcrime --in=RebalData/Chicago_bs60Top3_Rebal_sgd.model --out=RebalData/Chicago_bs60Top3Rebal_sgd.txt
./tstcrime --in=RebalData/Chicago_bs60Top5_Rebal_sgd.model --out=RebalData/Chicago_bs60Top5Rebal_sgd.txt
