# Graphs

## Input file format

Must be .txt

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

## Command line

When parsing the graph, pass in flags to indicate whether the graph is directed/undirected and weighted/unweighted

```bash
./graph.exe [-d] [-w] [*.txt files...]
```

- -d for directed, omit for undirected
- -w for weighted, omit for unweighted