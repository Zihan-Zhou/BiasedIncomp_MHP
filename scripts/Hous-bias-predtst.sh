
############## biased sampling: random select from other events ##########

./runcrime --config=cfg-Houston-Bias.txt --berno_rt=0.2 --top=1 --out=BiasData/h0Hou_OldFexp1_top1Bias20.model
./runcrime --config=cfg-Houston-Bias.txt --berno_rt=0.4 --top=1 --out=BiasData/h0Hou_OldFexp1_top1Bias40.model
./runcrime --config=cfg-Houston-Bias.txt --berno_rt=0.6 --top=1 --out=BiasData/h0Hou_OldFexp1_top1Bias60.model
./runcrime --config=cfg-Houston-Bias.txt --berno_rt=0.6 --top=2 --out=BiasData/h0Hou_OldFexp1_top2Bias60.model
./runcrime --config=cfg-Houston-Bias.txt --berno_rt=0.6 --top=3  --out=BiasData/h0Hou_OldFexp1_top3Bias60.model
./runcrime --config=cfg-Houston-Bias.txt --berno_rt=0.6 --top=5  --out=BiasData/h0Hou_OldFexp1_top5Bias60.model

./tstcrime --in=BiasData/h0Hou_OldFexp1_top1Bias20.model --out=BiasData/h0Hou_OldFexp1_top1Bias20.txt
./tstcrime --in=BiasData/h0Hou_OldFexp1_top1Bias40.model --out=BiasData/h0Hou_OldFexp1_top1Bias40.txt
./tstcrime --in=BiasData/h0Hou_OldFexp1_top1Bias60.model --out=BiasData/h0Hou_OldFexp1_top1Bias60.txt
./tstcrime --in=BiasData/h0Hou_OldFexp1_top2Bias60.model --out=BiasData/h0Hou_OldFexp1_top2Bias60.txt
./tstcrime --in=BiasData/h0Hou_OldFexp1_top3Bias60.model --out=BiasData/h0Hou_OldFexp1_top3Bias60.txt
./tstcrime --in=BiasData/h0Hou_OldFexp1_top5Bias60.model --out=BiasData/h0Hou_OldFexp1_top5Bias60.txt
