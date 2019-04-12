# -----------
# User Instructions:
# 
# Modify the function search so that it returns
# a table of values called expand. This table
# will keep track of which step each node was
# expanded.
#
# Make sure that the initial cell in the grid 
# you return has the value 0.
# ----------

grid = [[0, 0, 1, 0, 0, 0],
        [0, 0, 0, 0, 0, 0],
        [0, 0, 1, 0, 1, 0],
        [0, 0, 1, 0, 1, 0],
        [0, 0, 1, 0, 1, 0]]
init = [0, 0]
goal = [len(grid)-1, len(grid[0])-1]
cost = 1

delta = [[-1, 0], # go up
         [ 0,-1], # go left
         [ 1, 0], # go down
         [ 0, 1]] # go right

delta_name = ['^', '<', 'v', '>']
expand = [[-1, -1, -1, -1, -1, -1],
        [-1, -1, -1, -1, -1, -1],
        [-1, -1, -1, -1, -1, -1],
        [-1, -1, -1, -1, -1, -1],
        [-1, -1, -1, -1, -1, -1]]
init = [0, 0]
def search(grid,init,goal,cost):
    closed = grid
    closed[init[0]][init[1]] = 1
    x = init[0]
    y = init[1]
    
    g = 0 #corresponds to cost
    open_list = [[g,x,y]]
    
    found = False  #Flag to determine when goal was found
    resign = False #Flag to determine when goal cannot be found
    count2 = 0
    while found is False and resign is False:
        #check for open elements
        if(len(open_list) == 0):
            resign = True
            print("Failed!")
        else:
            #remove element from open list
            open_list.sort()
            open_list.reverse()
            next_element = open_list.pop()
            
            x = next_element[1]
            y = next_element[2]
            g = next_element[0]
            expand[x][y] = count2
            count2 = count2+1
            
            #check if goall was reached
            if(x == goal[0] and y==goal[1]):
                found = True
                #print(next_element)

            else:
                for i in range(len(delta)): #iterate through all possible actions
                    x2 = x + delta[i][0]
                    y2 = y + delta[i][1]
                    #check position inside bounds
                    if(x2 < len(grid) and x2 >= 0 and y2<len(grid[0]) and y2>=0):
                        #check position open
                        if(closed[x2][y2] == 0):

                            g2 = g + cost
                            open_list.append([g2,x2,y2])
                            closed[x2][y2] = 1
    return expand
search(grid,init,goal,cost)
