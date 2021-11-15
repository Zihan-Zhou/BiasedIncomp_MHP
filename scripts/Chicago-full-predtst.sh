./runcrime --config=cfg-Chicago-hidden0.txt --lambda=100000 --out=FullData/Chicago_Full.model

./tstcrime --in=FullData/Chicago_Full.model --out=FullData/Chicago_Full.txt


##################### Stitch
./runcrime --config=cfg-Chicago-hidden0.txt --minT=0 --maxT=730 --lambda=100000 --out=FullData/Chicago_Sti20.model
./runcrime --config=cfg-Chicago-hidden0.txt --minT=0 --maxT=730 --lambda=100000 --out=FullData/Chicago_Sti40.model
./runcrime --config=cfg-Chicago-hidden0.txt --minT=0 --maxT=730 --lambda=150000 --out=FullData/Chicago_Sti60.model

./tstcrime --in=FullData/Chicago_Sti20.model --out=FullData/Chicago_Sti20.txt
./tstcrime --in=FullData/Chicago_Sti40.model --out=FullData/Chicago_Sti40.txt
./tstcrime --in=FullData/Chicago_Sti60.model --out=FullData/Chicago_Sti60.txt

