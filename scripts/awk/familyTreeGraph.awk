/^[0-9]/{

# File layout:
# Creating file for progam Graph Viz genome -> parent from philogenetic_tree.txt

	if($3 > 0) {
		if(!nodes[$3]){
			nodes[$3] = $3 OFS "[image=\"Organism_"$3".png\"]";
		}
		if(!nodes[$2]){
			nodes[$2] = $2 OFS "[image=\"Organism_"$2".png\"]";
		}
		connections[i] = $3 OFS " -> " OFS $2;
		i++
	}
	if($4 > 0) {
		if(!nodes[$4]){
			nodes[$4] = $4 OFS "[image=\"Organism_"$4".png\"]";
		}
		if(!nodes[$2]){
			nodes[$2] = $2 OFS "[image=\"Organism_"$2".png\"]";
		}
		connections[i] = $4 OFS " -> " OFS $2;
		i++;
	}
}

END {
	print "digraph G {";
	for (x in nodes) {
		print nodes[x];
	}
	for(c in connections) {
		print connections[c];
	}
	print "}";
}