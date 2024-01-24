import random

FilePath = "c54321.pat"
a = 12872
with open(FilePath , "a") as filewrite:
        for j in range (3):    
            if(j != 0):
                filewrite.write("	")
            for i in range (a):
                b = random.randint(0,1)
                filewrite.write(str(b) + " ")
            if(j != 3):
                filewrite.write('\n')   
        filewrite.write(".end")