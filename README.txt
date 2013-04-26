# Casey Scheide cscheide@ucsc.edu
# Taran Lu tjlu@ucsc.edu 
# GOLDENROD ENGINE

Description:    OpenGL application that reads in a a text file describing
                a tile layout. Renders tiles and allows for simple transformation
                of the minigolf "hole."                
            
Controls:       Object transformations:
                'o' = enter obeject transformation mode
                'x' = tranform along x-axis (r-click = move, l-click = rotate)
                'y' = tranform along y-axis (r-click = move, l-click = rotate)
                'z' = tranform along z-axis (r-click = move, l-click = rotate)
                
                Camera Controls (WIP):
                Use the GUI to rotate and zoom the camera (it stays oriented
                toward the level).
                *NOTE: the GUI appears behind the main glut window
            
Comments:       Some code was taken/modified from the example
                program used in CS160.                                        

                Build with Visual Studio 2010.
            
                Should be free of external dependencies.