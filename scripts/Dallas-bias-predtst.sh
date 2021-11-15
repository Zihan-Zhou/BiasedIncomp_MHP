
./runcrime --config=cfg-Dallas-bias.txt  --berno_rt=0.2 --top=1 --out=BiasData/h0DL_Rob1yrtop1Bias20.model
./runcrime --config=cfg-Dallas-bias.txt  --berno_rt=0.4 --top=1 --out=BiasData/h0DL_Rob1yrtop1Bias40.model
./runcrime --config=cfg-Dallas-bias.txt  --berno_rt=0.6 --top=1 --out=BiasData/h0DL_Rob1yrtop1Bias60.model

./runcrime --config=cfg-Dallas-bias.txt --berno_rt=0.4 --top=1 --out=BiasData/h0DL_top1Bias40.model
./runcrime --config=cfg-Dallas-bias.txt --berno_rt=0.6 --top=1 --out=BiasData/h0DL_top1Bias60.model
./runcrime --config=cfg-Dallas-bias.txt --berno_rt=0.8 --top=1 --out=BiasData/h0DL_top1Bias80.model



./tstcrime --in=BiasData/h0DL_Rob1yrtop1Bias20.model --out=BiasData/h0DL_Rob1yrtop1Bias20.txt
./tstcrime --in=BiasData/h0DL_Rob1yrtop1Bias40.model --out=BiasData/h0DL_Rob1yrtop1Bias40.txt
./tstcrime --in=BiasData/h0DL_Rob1yrtop1Bias60.model --out=BiasData/h0DL_Rob1yrtop1Bias60.txt
./tstcrime --in=BiasData/h0DL_top1Bias40.model --out=BiasData/h0DL_top1Bias40.txt
./tstcrime --in=BiasData/h0DL_top1Bias60.model --out=BiasData/h0DL_top1Bias60.txt
./tstcrime --in=BiasData/h0DL_top1Bias80.model --out=BiasData/h0DL_top1Bias80.txt
