./runcrime --config=cfg-Houston-hidden0.txt --lambda=1000 --minT=0 --maxT=21.0  --out=FullData/h0Hou_OldFexp1_Full.model


./tstcrime --in=FullData/h0Hou_OldFexp1_Full.model --out=FullData/h0Hou_OldFexp1_Full.txt


###### Stiched 
./runcrime --config=cfg-Houston-hidden0.txt --lambda=1000 --minT=0 --maxT=21.0  --out=FullData/h0Hou_OldFexp1_Sti20.model
./runcrime --config=cfg-Houston-hidden0.txt --lambda=1000 --minT=0 --maxT=21.0  --out=FullData/h0Hou_OldFexp1_Sti40.model
./runcrime --config=cfg-Houston-hidden0.txt --lambda=3000 --minT=0 --maxT=21.0  --out=FullData/h0Hou_OldFexp1_Sti60.model
./runcrime --config=cfg-Houston-hidden0.txt --lambda=3000 --minT=0 --maxT=21.0  --out=FullData/h0Hou_OldFexp1_Top2Sti60.model
./runcrime --config=cfg-Houston-hidden0.txt --lambda=3000 --minT=0 --maxT=21.0  --out=FullData/h0Hou_OldFexp1_Top3Sti60.model
./runcrime --config=cfg-Houston-hidden0.txt --lambda=3000 --minT=0 --maxT=21.0  --out=FullData/h0Hou_OldFexp1_Top5Sti60.model

./tstcrime --in=FullData/h0Hou_OldFexp1_Sti20.model --out=FullData/h0Hou_OldFexp1_Sti20.txt
./tstcrime --in=FullData/h0Hou_OldFexp1_Sti40.model --out=FullData/h0Hou_OldFexp1_Sti40.txt
./tstcrime --in=FullData/h0Hou_OldFexp1_Sti60.model --out=FullData/h0Hou_OldFexp1_Sti60.txt
./tstcrime --in=FullData/h0Hou_OldFexp1_Top2Sti60.model --out=FullData/h0Hou_OldFexp1_Top2Sti60.txt
./tstcrime --in=FullData/h0Hou_OldFexp1_Top3Sti60.model --out=FullData/h0Hou_OldFexp1_Top3Sti60.txt
./tstcrime --in=FullData/h0Hou_OldFexp1_Top5Sti60.model --out=FullData/h0Hou_OldFexp1_Top5Sti60.txt


