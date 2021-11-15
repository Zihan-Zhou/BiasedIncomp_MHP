

######## Top x + 40%/60% rare

./runcrime --config=cfg-Chicago-bias.txt --top=1 --berno_rt=0.2 --out=BiasData/Chicago_Top1Bias20.model
./runcrime --config=cfg-Chicago-bias.txt --top=1 --berno_rt=0.4 --out=BiasData/Chicago_Top1Bias40.model
./runcrime --config=cfg-Chicago-bias.txt --top=1 --berno_rt=0.6 --out=BiasData/Chicago_Top1Bias60.model
./runcrime --config=cfg-Chicago-bias.txt --top=2 --berno_rt=0.6 --out=BiasData/Chicago_Top2Bias60.model 
./runcrime --config=cfg-Chicago-bias.txt --top=3 --berno_rt=0.6 --out=BiasData/Chicago_Top3Bias60.model
./runcrime --config=cfg-Chicago-bias.txt --top=5 --berno_rt=0.6 --out=BiasData/Chicago_Top5Bias60.model

./tstcrime --in=BiasData/Chicago_Top1Bias20.model --out=BiasData/Chicago_Top1Bias20.txt
./tstcrime --in=BiasData/Chicago_Top1Bias40.model --out=BiasData/Chicago_Top1Bias40.txt
./tstcrime --in=BiasData/Chicago_Top1Bias60.model --out=BiasData/Chicago_Top1Bias60.txt
./tstcrime --in=BiasData/Chicago_Top2Bias60.model --out=BiasData/Chicago_Top2Bias60.txt
./tstcrime --in=BiasData/Chicago_Top3Bias60.model --out=BiasData/Chicago_Top3Bias60.txt
./tstcrime --in=BiasData/Chicago_Top5Bias60.model --out=BiasData/Chicago_Top5Bias60.txt
