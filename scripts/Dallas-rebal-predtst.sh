################ AAAI Method: 5 hidden nodes ##############
./runcrime --config=cfg-Dallas-hidden5.txt --berno_rt=0.2 --top=1 --out=RebalData/h0DL_1yrtop1_aaai20.model
./runcrime --config=cfg-Dallas-hidden5.txt --berno_rt=0.4 --top=1 --out=RebalData/h0DL_1yrtop1_aaai40.model
./runcrime --config=cfg-Dallas-hidden5.txt --berno_rt=0.6 --lambda=2000 --top=1 --out=RebalData/h0DL_1yrtop1_aaai60.model



./tstcrime --in=RebalData/h0DL_1yrtop1_aaai20.model --out=RebalData/h0DL_1yrtop1_aaai20.txt
./tstcrime --in=RebalData/h0DL_1yrtop1_aaai40.model --out=RebalData/h0DL_1yrtop1_aaai40.txt
./tstcrime --in=RebalData/h0DL_1yrtop1_aaai60.model --out=RebalData/h0DL_1yrtop1_aaai60.txt

################ Our Method: Missing Windows ##############

./runcrime --config=cfg-Dallas-sgd.txt --berno_rt=0.2 --top=1 --out=RebalData/h0DL_1yrtop1_sgd20.model
./runcrime --config=cfg-Dallas-sgd.txt --berno_rt=0.4 --top=1 --out=RebalData/h0DL_1yrtop1_sgd40.model
./runcrime --config=cfg-Dallas-sgd.txt --berno_rt=0.6 --top=1 --out=RebalData/h0DL_1yrtop1_sgd60.model


./tstcrime --in=RebalData/h0DL_1yrtop1_sgd20.model --out=RebalData/h0DL_1yrtop1_sgd20.txt
./tstcrime --in=RebalData/h0DL_1yrtop1_sgd40.model --out=RebalData/h0DL_1yrtop1_sgd40.txt
./tstcrime --in=RebalData/h0DL_1yrtop1_sgd60.model --out=RebalData/h0DL_1yrtop1_sgd60.txt