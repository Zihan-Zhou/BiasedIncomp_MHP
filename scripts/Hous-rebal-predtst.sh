################ AAAI Method: 5 hidden nodes ##############
./runcrime --config=cfg-Houston-hidden5.txt --berno_rt=0.2 --top=1 --out=RebalData/h0Hou_OldFexp1_hd5_2bstop1_Rebal_orig.model
./runcrime --config=cfg-Houston-hidden5.txt --berno_rt=0.4 --top=1 --out=RebalData/h0Hou_OldFexp1_hd5_4bstop1_Rebal_orig.model
./runcrime --config=cfg-Houston-hidden5.txt --berno_rt=0.6 --top=1 --out=RebalData/h0Hou_OldFexp1_hd5_6bstop1_Rebal_orig.model
./runcrime --config=cfg-Houston-hidden5.txt --berno_rt=0.6 --top=2 --out=RebalData/h0Hou_OldFexp01_hd5_6bstop2_Rebal_orig.model
./runcrime --config=cfg-Houston-hidden5.txt --berno_rt=0.6 --top=3 --out=RebalData/h0Hou_OldFexp1_hd5_6bstop3_Rebal_orig.model
./runcrime --config=cfg-Houston-hidden5.txt --berno_rt=0.6 --top=5 --out=RebalData/h0Hou_OldFexp1_hd5_6bstop5_Rebal_orig.model

./tstcrime --in=RebalData/h0Hou_OldFexp1_hd5_2bstop1_Rebal_orig.model --out=RebalData/h0Hou_OldFexp1_hd5_2bstop1_Rebal_orig.txt
./tstcrime --in=RebalData/h0Hou_OldFexp1_hd5_4bstop1_Rebal_orig.model --out=RebalData/h0Hou_OldFexp1_hd5_4bstop1_Rebal_orig.txt
./tstcrime --in=RebalData/h0Hou_OldFexp1_hd5_6bstop1_Rebal_orig.model --out=RebalData/h0Hou_OldFexp1_hd5_6bstop1_Rebal_orig.txt
./tstcrime --in=RebalData/h0Hou_OldFexp1_hd5_6bstop2_Rebal_orig.model --out=RebalData/h0Hou_OldFexp1_hd5_6bstop2_Rebal_orig.txt
./tstcrime --in=RebalData/h0Hou_OldFexp1_hd5_6bstop3_Rebal_orig.model --out=RebalData/h0Hou_OldFexp1_hd5_6bstop3_Rebal_orig.txt
./tstcrime --in=RebalData/h0Hou_OldFexp1_hd5_6bstop5_Rebal_orig.model --out=RebalData/h0Hou_OldFexp1_hd5_6bstop5_Rebal_orig.txt


################ Our Method: Missing Windows ##############
./runcrime --config=cfg-Houston-sgd.txt --lambda=3000 --lambda2=5000 --berno_rt=0.2 --top=1 --out=RebalData/h0Hou_OldFexp1_20bstop1_Rebal_sgd.model
./runcrime --config=cfg-Houston-sgd.txt --lambda=200 --lambda2=2000 --berno_rt=0.4 --top=1 --out=RebalData/h0Hou_OldFexp1_40bstop1_Rebal_sgd.model
./runcrime --config=cfg-Houston-sgd.txt --lambda=3000 --lambda2=3500 --berno_rt=0.6 --top=1 --out=RebalData/h0Hou_OldFexp1_60bstop1_Rebal_sgd.model
./runcrime --config=cfg-Houston-sgd.txt --lambda=2000 --lambda2=5000 --berno_rt=0.6 --top=1 --out=RebalData/h0Hou_OldFexp1_6bstop1_Rebal_sgd.model
./runcrime --config=cfg-Houston-sgd.txt --lambda=2000 --lambda2=5000 --berno_rt=0.6 --top=2 --out=RebalData/h0Hou_OldFexp01_6bstop2_Rebal_sgd.model
./runcrime --config=cfg-Houston-sgd.txt --lambda=2000 --lambda2=5000 --berno_rt=0.6 --top=3 --out=RebalData/h0Hou_OldFexp1_6bstop3_Rebal_sgd.model



./tstcrime --in=RebalData/h0Hou_OldFexp1_20bstop1_Rebal_sgd.model --out=RebalData/h0Hou_OldFexp1_20bstop1_Rebal_sgd.txt --outID=RebalData/IDHou_OldFexp1_20bstop1_Rebal_sgd.txt
./tstcrime --in=RebalData/h0Hou_OldFexp1_40bstop1_Rebal_sgd.model --out=RebalData/h0Hou_OldFexp1_40bstop1_Rebal_sgd.txt --outID=RebalData/IDHou_OldFexp1_40bstop1_Rebal_sgd.txt
./tstcrime --in=RebalData/h0Hou_OldFexp1_60bstop1_Rebal_sgd.model --out=RebalData/h0Hou_OldFexp1_60bstop1_Rebal_sgd.txt --outID=RebalData/IDHou_OldFexp1_60bstop1_Rebal_sgd.txt
./tstcrime --in=RebalData/h0Hou_OldFexp1_6bstop1_Rebal_sgd.model --out=RebalData/h0Hou_OldFexp1_6bstop1_Rebal_sgd.txt
./tstcrime --in=RebalData/h0Hou_OldFexp1_6bstop2_Rebal_sgd.model --out=RebalData/h0Hou_OldFexp1_6bstop2_Rebal_sgd.txt
./tstcrime --in=RebalData/h0Hou_OldFexp1_6bstop3_Rebal_sgd.model --out=RebalData/h0Hou_OldFexp1_6bstop3_Rebal_sgd.txt

