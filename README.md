# Graphs

## Input file format

- Vertices are 1-indexed

```bash
12 # Number of vertices
1 2 # Define an edge from vertex 1 to vertex 2
1 5
3 4
3 7
3 8
4 8
5 9
5 10
7 8
7 11
8 11
8 12
9 10
```

### Weighted format

```bash
10
2 7 419 # Define an edge from vertex 2 to vertex 7 with weight 419
8 9 803
6 7 184
4 6 152
2 9 260
5 9 741
3 10 842
5 7 802
9 10 483
1 8 410
2 3 581
1 7 426
3 7 53
3 5 453
6 8 50
```

### Optional labeling format

```bash
10
- # Start labels
S 1
A 2
B 3
C 4
D 5
E 6
F 7
G 8
H 9
I 10
- # End labels
A F 419 # Define an edge from vertex A (2) to vertex F (7) with weight 419
G H 803
E F 184
C E 152
A H 260
D H 741
B I 842
D F 802
H I 483
S G 410
A B 581
S F 426
B F 53
B D 453
E G 50
```

## Command line

When parsing the graph, pass in flags to indicate whether the graph is directed/undirected and weighted/unweighted

```bash
./graph.exe [-d] [-w] [-p] [-e] [files...]
```

- -d for directed, omit for undirected
- -w for weighted, omit for unweighted
- -p just print graphs, don't write to file
- -e print edges too