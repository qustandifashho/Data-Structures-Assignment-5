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
            if (cell == 'X'){
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
    Graph* graph = buildGraph(maze);
    dijkstrasAlg(graph, createPoint(0,0)); // dij does shortest path 
    double distanceToEnd = getDistance(graph, createPoint(0,0), createPoint(maze->length - 1, maze->width - 1));



    // free
    freeGraph(graph);
    //determine of the path exists:
    if(distanceToEnd == INT_MAX){
        return PATH_IMPOSSIBLE;
    }
    else{
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
    //TODO: Complete this function
    /* HINT 1: To solve this, my solution used the functions createGraph, freeGraph, setEdge, dijkstrasAlg, getDistance from graph.c */
    /* HINT 2: My solution also used createPoint from point2D.c */
    /* HINT 3: You might also consider using the new helper function buildGraph to build the graph representing maze. */
    
    Graph* graph = buildGraph(maze);

    // dij finds shortest path 
    dijkstrasAlg(graph, createPoint(0,0));

    //Finding nearest 'F' and its distance 
    int finishRow = -1;
    int finishColumn = -1;
    double nearestDistanceToFinish = INT_MAX;
    double distance;
    int i = 0; // to length of maze 
    int j = 0; // to width of maze 

    for(i = 0; i < maze->length; i++){
        for(j = 0; j < maze->width; j++){
            if(maze->array2D[i][j] == 'F'){
                distance = getDistance(graph, createPoint(0,0), createPoint(i,j)); 
                if(distance < nearestDistanceToFinish){
                    nearestDistanceToFinish = distance;
                    finishRow = i;
                    finishColumn = j;
                }
            }
        }
    }

    if(nearestDistanceToFinish == INT_MAX){
        (*spDist) = INT_MAX;
        free(graph);
        return PATH_IMPOSSIBLE;
    }
    else{
        // updating spdist with the shortest path distance 
        (*spDist) = (int)nearestDistanceToFinish;
        free(graph);
        return PATH_FOUND;
    }

    
    
    //(*spDist) = INT_MAX; /* TODO: This returns your shortest path distance to any 'F' from the 'S'.  Set it to INT_MAX if no path exists. */
     
   
    
    //return PATH_UNKNOWN; /* TODO: Replace with PATH_FOUND or PATH_IMPOSSIBLE based on whether a path exists */
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
    Graph* graph = buildGraph(maze);

    // Dijkstrra maybe? to find shortest path 
    dijkstrasAlg(graph, createPoint(0,0));

    //count number of x in the path 
    int tunnelCount = 0;
    int currRow = maze->length-1;
    int currCol = maze->width-1;
    Point2D prev;


    while(currRow != 0|| currCol != 0){
        if(maze->array2D[currRow][currCol] == 'X'){
            tunnelCount++;
        }
        getPredecessor(graph, createPoint(currRow, currCol), &prev);
        currRow = prev.x;
        currCol = prev.y;
    
        

    }
    free(graph);

    if (tunnelCount > k) {
        return PATH_IMPOSSIBLE;
    } else {
        return PATH_FOUND;
    }


   
    
    //return PATH_UNKNOWN; /* TODO: Replace with PATH_FOUND or PATH_IMPOSSIBLE based on whether a path exists */
}
