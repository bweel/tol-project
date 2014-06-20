/^[0-9]/ {
	organism = substr(FILENAME,12,match(FILENAME,"[0-9]/")-11);
	
	if(!fitness[organism][1]) {
		fitness[organism][1] = 0;
		fitness[organism][2] = 0;
	}
	
	if(FNR>=2 && FNR<=27){
		fitness[organism][1] += $6
	}
	if(FNR>=175 && FNR<=200)
	{
		fitness[organism][2] += $6
	}
}

END {
	for(x in fitness){
		print x,fitness[x][1]/25,fitness[x][2]/25,(fitness[x][2]/25)-(fitness[x][1]/25);
	}
}