# data-structure-visualizer
Data structure visualizer for CS163.

# User Manual

Open the `app.exe` file to open the app.

Some test data is in `data` folder, with the first one of each type is a valid file.

## Singly linked list

- Insert: Insert the value from the input field.
- Delete: Delete the value from the input field if exist in the data structure.
- Find: Find the value from the input field.
- Update: Delete the value from the input field. If deletion is success, insert a new element from the `to` field.
- Clear: Clear the data structure.
- Random: Generate a structure with random elements.
- File: Generate a structure with elements from file.
- Reset: Reset the view.

## Heap

- Insert: Insert the value from the input field.
- Id remove: Remove the id-th element in the heap if id is less than the number of element
- Pop: Pop the highest element.
- Id update: Update the id-th element to new value from `to` field if id is less than the number of element.
- Clear: Clear the data structure.
- Random: Generate a structure with random elements.
- File: Generate a structure with elements from file.
- Reset: Reset the view.

## AVL tree

- Insert: Insert the value from the input field.
- Delete: Delete the value from the input field if exist in the data structure.
- Find: Find the value from the input field.
- Update: Delete the value from the input field. If deletion is success, insert a new element from the `to` field.
- Clear: Clear the data structure.
- Random: Generate a structure with random elements.
- File: Generate a structure with elements from file.
- Reset: Reset the view.

## Trie

- Insert: Insert the value from the input field.
- Delete: Delete the value from the input field if exist in the data structure.
- Find: Find the value from the input field.
- Update: Delete the value from the input field. If deletion is success, insert a new element from the `to` field.
- Clear: Clear the data structure.
- Random: Generate a structure with random elements.
- File: Generate a structure with elements from file.
- Reset: Reset the view.

## Graph
- Update: Update the graph structure. The convention is the same as input file.
- Prim: Run the Prim's algorithm.
- Dijkstra: Run the Dijkstra's algorithm. The starting node is from the `from` field, the ending node is from the `to` field.
- Clear: Clear the graph.
- Random: Randomly generate a graph.
- File: Generate graph from file.
- Reset: Reset the view.

# Input file convention

## Singly linked list

```
n
a_1 a_2 a_3 ... a_n
```
With:
- `n`: The number of elements
- `a_i`: The element

Example:
```
6
2 100 2 65 1 39
```

## Heap

```
n
a_1 a_2 a_3 ... a_n
```
With:
- `n`: The number of elements
- `a_i`: The element

Example:
```
6
2 100 2 65 1 39
```

## AVL tree

```
n
a_1 a_2 a_3 ... a_n
```
With:
- `n`: The number of elements
- `a_i`: The element

Example:
```
6
2 100 2 65 1 39
```

Note that for duplicate elements, only the first one is added into the structure.

## Trie

```
n
s_1
s_2
s_3
...
s_n
```
With:
- `n`: The number of elements
- `s_i`: The element

Example:
```
6
abcde
xycscs
quick
hcmushcmushcmus
hcmushcmushcmus
a
```

## Graph

```
n m
u_1 v_1 w_1
u_2 v_2 w_2
u_3 v_3 w_3
...
u_m v_m w_m
```
With:
- `n`: The number of nodes (`0...n-1`)
- `m`: The number of edges
- `u_i v_i w_i`: bidirectional edge between `u_i` and `v_i` with weight `w_i`

Example:
```
5 7
0 2 75
0 1 9
2 1 95
3 2 51
3 1 19
1 4 42
3 4 31
```

