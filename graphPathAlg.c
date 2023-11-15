#include "graph.h"
#include "graphPathAlg.h"


/* printName
 * input: none
 * output: none
 *
 * Prints the name of the student who worked on this solution
 */
void printName( )
{
    /* TODO : Fill in your names */
    printf("This solution was completed by:\n");
    printf("Qustandi Fashho\n");
}

/* OPTIONAL HELPER FUNCTION
 * buildGraph
 *
 * IMPORTANT NOTE: This is an entirely optional helper function which is only called by student implemented functions.
 * Creates a new graph to represent the given maze.  
 */
Graph* buildGraph( array2D* maze /* and other parameters you find helpful */  )
{
    //OPTIONAL TODO: Translate the given maze into a graph.  'X' represents impassable locations.  Only moves of up, down, left, and right are allowed. 
    /* With the right parameters this can be used by all three functions below to build the graph representing their different maze problems. */
    /* HINT 1: To solve this, my solution used the functions createGraph and setEdge from graph.c */
    /* HINT 2: My solution also used createPoint from point2D.c */

    int rows = maze->length;
    int cols = maze ->width;
    Graph* graph = createGraph(rows*cols);

    int i = 0;
    int j = 0;
    for(i = 0; i < rows; i++){
        for(j = 0; j < cols; j++){
            char cell = maze->array2D[i][j];
            if (cell =='X'){
                continue;
            }
             // Create points for the current cell and its neighboring cells
            Point2D currentPoint = createPoint(i, j);

            // Connect to the cell above (if within bounds and not 'X')
            if (i > 0 && maze->array2D[i - 1][j] != 'X') {
                Point2D neighborPoint = createPoint(i - 1, j);
                setEdge(graph, currentPoint, neighborPoint, 1.0);
            }

            // Connect to the cell below (if within bounds and not 'X')
            if (i < rows - 1 && maze->array2D[i + 1][j] != 'X') {
                Point2D neighborPoint = createPoint(i + 1, j);
                setEdge(graph, currentPoint, neighborPoint, 1.0);
            }

            // Connect to the cell on the left (if within bounds and not 'X')
            if (j > 0 && maze->array2D[i][j - 1] != 'X') {
                Point2D neighborPoint = createPoint(i, j - 1);
                setEdge(graph, currentPoint, neighborPoint, 1.0);
            }

            // Connect to the cell on the right (if within bounds and not 'X')
            if (j < cols - 1 && maze->array2D[i][j + 1] != 'X') {
                Point2D neighborPoint = createPoint(i, j + 1);
                setEdge(graph, currentPoint, neighborPoint, 1.0);
            }
        }
    }
       
    

    return graph; /* TODO: Replace with your graph representing maze */
}

/* hasPath
 * input: an array2D pointer to a maze
 * output: pathResult
 *
 * Detects whether a path exists from 'S' to 'F' in the graph. 'X' marks impassable regions.
 */
pathResult hasPath( array2D *maze )
{
     //TODO: Complete this function
    /* HINT 1: To solve this, my solution used the functions createGraph, freeGraph, setEdge, dijkstrasAlg, getDistance from graph.c */
    /* HINT 2: My solution also used createPoint from point2D.c */
    /* HINT 3: You might also consider using the new helper function buildGraph to build the graph representing maze. */


    // make sure I used all the functions mentioned
    Graph *graph = buildGraph(maze);

    Point2D start, finish;

    // Find the start and finish points in the maze
    for (int i = 0; i < maze->length; i++)
    {
        for (int j = 0; j < maze->width; j++)
        {
            if (maze->array2D[i][j] == 'S')
            {
                start = createPoint(i, j);
            }
            else if (maze->array2D[i][j] == 'F')
            {
                finish = createPoint(i, j);
            }
        }
    }

    dijkstrasAlg(graph, start); // dij does shortest path
    double distanceToEnd = getDistance(graph, start, finish);

    // free
    freeGraph(graph);
    //determine if the path exists:
    if (distanceToEnd == INT_MAX)
    {
        return PATH_IMPOSSIBLE;
    }
    else
    {
        return PATH_FOUND;
    }

    //return PATH_UNKNOWN; /* TODO: Replace with PATH_FOUND or PATH_IMPOSSIBLE based on whether a path exists */
}




















/* findNearestFinish
 * input: an array2D pointer to a maze, a pointer to an int
 * output: pathResult
 *
 * The maze contains one 'S' and multiple 'F's (1 or more).  'X' marks impassable regions.
 * Find the length of the shortest path to any 'F' from 'S' and return it in spDist.
 * If no 'F' is reachable set spDist to INT_MAX.
 */
pathResult findNearestFinish( array2D *maze, int *spDist )
{
    // TODO: Complete this function
    /* HINT 1: To solve this, my solution used the functions createGraph, freeGraph, setEdge, dijkstrasAlg, getDistance from graph.c */
    /* HINT 2: My solution also used createPoint from point2D.c */
    /* HINT 3: You might also consider using the new helper function buildGraph to build the graph representing maze. */


    Graph* graph = buildGraph(maze);

    // Find the start point in the maze
    Point2D start;
    int i, j;

    for (i = 0; i < maze->length; i++) {
        for (j = 0; j < maze->width; j++) {
            if (maze->array2D[i][j] == 'S') {
                start = createPoint(i, j);
                break; // Stop searching once 'S' is found
            }
        }
    }

    int minDistance = INT_MAX;

    // Loop through all 'F' positions and find the minimum distance
    for (i = 0; i < maze->length; i++) {
        for (j = 0; j < maze->width; j++) {
            if (maze->array2D[i][j] == 'F') {
                Point2D finish = createPoint(i, j);

                // Dijkstra finds the shortest path
                dijkstrasAlg(graph, start);

                // Find the distance to the current 'F'
                double distanceToFinish = getDistance(graph, start, finish);

                // Update minDistance if the current distance is smaller
                if (distanceToFinish < minDistance) {
                    minDistance = (int)distanceToFinish;
                }
            }
        }
    }

    freeGraph(graph);

    if (minDistance == INT_MAX) {
        *spDist = INT_MAX;
        return PATH_IMPOSSIBLE;
    } else {
        // Update spDist with the shortest path distance
        *spDist = minDistance;
        return PATH_FOUND;
    }
}










/* findTunnelRoute
 * input: an array2D pointer to a maze, an int representing the number X's you can travel through
 * output: pathResult
 *
 * Detects whether a path exists from 'S' to 'F' in the graph where you pass through at most k 'X' symbols.
 */
pathResult findTunnelRoute( array2D *maze, int k )
{

    //TODO: Complete this function
    /* HINT 1: To solve this, my solution used the functions createGraph, freeGraph, setEdge, dijkstrasAlg, getDistance from graph.c */
    /* HINT 2: My solution also used createPoint from point2D.c */
    /* HINT 3: You might also consider using the new helper function buildGraph to build the graph representing maze. */

    // You will need to build a graph which is slightly different from part 1 and 2 (currently it looks like you build the same graph).  
    // Specifically, for part 3, think about what the cost should be for an edge going through a 'X' and what the cost should be for an edge going through a ' '.  
    // Then, when you find the shortest path length from 'S' to 'F', it will be the number of walls you need to tunnel through.

    
//     Graph* graph = buildGraph(maze);

//     // Dijkstrra maybe? to find shortest path 
//     dijkstrasAlg(graph, createPoint(0,0));

//     //count number of x in the path 
//     int tunnelCount = 0;
//     int currRow = maze->length-1;
//     int currCol = maze->width-1;
//     Point2D prev;


//     while(currRow != 0|| currCol != 0){
//         if(maze->array2D[currRow][currCol] == 'X'){
//             tunnelCount++;
//         }
//         getPredecessor(graph, createPoint(currRow, currCol), &prev);
//         currRow = prev.x;
//         currCol = prev.y;
    
        

//     }
//     freeGraph(graph);

//     if (tunnelCount > k) {
//         return PATH_IMPOSSIBLE;
//     } else {
//         return PATH_FOUND;
//     }


   
    
//     //return PATH_UNKNOWN; /* TODO: Replace with PATH_FOUND or PATH_IMPOSSIBLE based on whether a path exists */
// }








    int i = 0; // i is row
    int j = 0; // j is column 
    //int tunnelCount = 0;
    int ret;
    int startFound;
    int finishFound;
    Point2D start, finish;

    Graph* graph;
    graph = createGraph(maze->length * maze->width);
    for( i = 1; i < maze->length-1; i++){
        for(j = 1; j < maze->width-1; j++){
            // You need to find 'S' and 'F' coordinates in the maze
            if (maze->array2D[i][j] == 'S') {
                start = createPoint(i, j);
                startFound = 1;
        }   
            else if (maze->array2D[i][j] == 'F') {
                finish = createPoint(i, j);
                finishFound = 1;
        }
            if(maze->array2D[i][j] != 'X'){ // if cahracter is not X
                if(maze->array2D[i][j+1] != 'X'){ // j + 1 is column + 1 so go up
                    setEdge(graph, createPoint(i, j), createPoint(i, j+1), 1);
                }
                if(maze->array2D[i-1][j] != 'X'){ // row - 1 so go left 
                    setEdge(graph, createPoint(i, j), createPoint(i-1, j), 1 );
                }
                if(maze->array2D[i+1][j] != 'X'){ // row + 1 so go right 
                    setEdge(graph, createPoint(i,j), createPoint(i+1, j), 1 );
                }
                if(maze->array2D[i][j-1] != 'X'){ // column + 1 so go up
                    setEdge(graph, createPoint(i,j), createPoint(i, j+1), 1);
                }

            }
            else if(maze->array2D[i][j] == 'X'){ // if the character is X
                if(maze->array2D[i][j+1] == 'X'){ // j + 1 is column + 1 so go up
                    setEdge(graph, createPoint(i, j), createPoint(i, j+1), 0);
                }
                if(maze->array2D[i-1][j] == 'X'){ // row - 1 so go left 
                    setEdge(graph, createPoint(i, j), createPoint(i-1, j), 0 );
                }
                if(maze->array2D[i+1][j] == 'X'){ // row + 1 so go right 
                    setEdge(graph, createPoint(i,j), createPoint(i+1, j), 0 );
                }
                if(maze->array2D[i][j-1] == 'X'){ // column + 1 so go up
                    setEdge(graph, createPoint(i,j), createPoint(i, j+1), 0);
                }
        }
    }

}
    if (!startFound || !finishFound) {
    // Handle error: 'S' or 'F' not found
        freeGraph(graph);
        return PATH_IMPOSSIBLE;
    }
    else{
        freeGraph(graph);
        return PATH_FOUND;
    }

    dijkstrasAlg(graph, start );
    ret = getDistance(graph, start, finish);
    freeGraph(graph);
    return ret;

}

//     Graph* g = createGraph(maze->length * maze ->width);
//     int ret, row, col;
//     Point2D start, finish;
//     for(row = 1; row < maze->length-1; row++){
//         for(col = 1; col< maze->width-1; col++){
//             Point2D next = createPoint(row, col);
//             for(k = 0; maze->array2D[row-k][col-k]!='X'; k++){
//                 setEdge(g, next, createPoint(row-k, col-k), 1);
//             }
//             for(k = 0; maze->array2D[row-k][col+k]!='X'; k++){
//                 setEdge(g, next, createPoint(row-k, col+k), 1);
//             }
//             for(k = 0; maze->array2D[row+k][col-k]!='X'; k++){
//                 setEdge(g, next, createPoint(row+k, col+k), 1);
//             }
//             for(k = 0; maze->array2D[row+k][col+k]!='X'; k++){
//                 setEdge(g, next, createPoint(row+k, col+k), 1);
//             }
//     }

//     }
//     dijkstrasAlg(g, start);
//     ret = getDistance(g, start, finish);
//     freeGraph(g);

//     return ret;
// }
