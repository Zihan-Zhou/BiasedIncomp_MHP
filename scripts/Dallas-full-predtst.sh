./runcrime --config=cfg-Dallas-hidden0.txt --maxT=650  --out=FullData/Dallas_Rob1yrFull.model --out_Loglike=FullData/LoglikeDallas_Rob1yrFull.txt


./tstcrime --in=FullData/Dallas_Rob1yrFull.model --out=FullData/Dallas_Rob1yrFull.txt --outID=FullData/ID_DLRob1yrFull.txt


###### Stiched
./runcrime cfg-Dallas-hidden0.txt --maxT=650  --out=FullData/Dallas_Rob1yr_Sti20.model
./runcrime cfg-Dallas-hidden0.txt --maxT=650  --out=FullData/Dallas_Rob1yr_Sti40.model
./runcrime cfg-Dallas-hidden0.txt --maxT=650  --out=FullData/Dallas_Rob1yr_Sti60.model

./tstcrime --in=FullData/Dallas_Rob1yr_Sti20.model --out=FullData/Dallas_Rob1yr_Sti20.txt
./tstcrime --in=FullData/Dallas_Rob1yr_Sti40.model --out=FullData/Dallas_Rob1yr_Sti40.txt
./tstcrime --in=FullData/Dallas_Rob1yr_Sti60.model --out=FullData/Dallas_Rob1yr_Sti60.txt
