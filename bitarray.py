#Created by Jacob "qwertey6" Kaplan for Artificial Intelligence at WPI.
#This is a C pre-processor which allows you to create bit arrays as a primitive type.
import math

BIT_ARRAY_TYPE = "long long unsigned int"

COMMON_CODE_FOR_INSERTION ="""
#define unfilled 0
#define white 1
#define black 2
#ifndef BLOCK_SIZE
#define BLOCK_SIZE 15
#endif
#ifndef INT_SIZE
#define INT_SIZE 64
#endif
"""

#we will write custom functions for each bitarray, reducing lookups, and leaving our lookups virtually one dimensional.
DYNAMIC_CODE_FOR_INSERTION = """
inline int get__{BITARRAY_ID}(int y, int x){
    //which int we will look at for retrieving the bitarray value
    int intID = (x*BLOCK_SIZE + y)/INT_SIZE;

    //how many bits we will shift our selector before grabbing our bits
    int intoffset = (x*BLOCK_SIZE + y)%9;
    return ({BITARRAY_ID}[intID] & (3 << intoffset)) >> intoffset;
};
inline void set__{BITARRAY_ID}(int y, int x, int value){
    //which int we will look at for setting the bitarray value
    int intID = (x*BLOCK_SIZE + y)/INT_SIZE;

    //how many bits we will shift our selector before grabbing our bits
    int intoffset = (x*BLOCK_SIZE + y)%9;
    {BITARRAY_ID}[intID] = ({BITARRAY_ID}[intID] & ~(3 << intoffset)) | (value & (3 << intoffset));
};
""".replace("){","){{").replace("};","}};")#the replace statements escape the braces for python (removed on finish)


def preprocess(inputfile, outputfile):

    with open(inputfile, mode="r") as f:
        code = f.read()
        f.close()
    
    def between(string, sub0, sub1):
        return string[string.index(sub0)+1:string.index(sub1)]
        
    #we need to keep track of which variable(s) are bitarrays, their
    #maximum sizes, and the 'long long uint's which they will be expanded into
    bitarrays = {}

    ba = "bitarray"
    var_ID = -1
    #in this while loop, we will populate our bitarrays dictionary, mapping variables to the bitarrays.
    while code.count("bitarray"):
        #(note: we do not care if "bitarray" occurs in a comment -- this is right before pre-processing, and accidental comment expansion is inconsequential)
        var_ID += 1# add 1 to the ID of the current variable we are looking at.


        i = code.index(ba)#index of "b" in bitarray
        j = code.index(";", i)+1#index of the first ";" after the "b" in bitarray

        #take the all the code before and after this line -- excising the current line.
        tail, line, head = code[0:i]+"\n", code[i:j].strip(), "\n"+code[j:]#also take the current line and strip it of whitespace
        
        line_parts = line.split()#split the line on internal whitespace
        prepend = ''#the previous line-part to prepend onto the current line-part, if necessary
        checked_line = []#the final line, after it has been parsed
        for part in line_parts:

            part = prepend+part#prepend the previous part, if we had one

            #verify that all brackets/braces are matched
            #(since the CPP will _not_ tell us if we have a bitarray related syntax error.)
            if part.count('[') != part.count(']'):
                #if we do not match bracket counts, then merge this part with the next
                prepend = part
                continue
            if part.count('(') != part.count(')'):
                #if we do not match parenthesis counts, then merge this part with the next
                prepend = part
                continue

            prepend = ''#if we reach the end of this loop, then we must have nothing to prepend
            checked_line.append(part)#as such, this part of the line has been parsed properly
        
        if checked_line[-1][-1] == ';':#make sure this line is properly terminated with a semicolon
            checked_line[-1] = checked_line[-1][0:-1]
        else: print("ERROR, A BITARRAY LINE DOES NOT END IN A SEMICOLON.")

        # add an entry to the dictionary of the variable, and how many long long uints it requires to be stored.
        bitarrays[var_ID] = {'BITARRAY_ID':checked_line[1], 'NUM_LONGS':math.ceil(eval(between(checked_line[0],'[',']'))/64)}

        REPLACEMENT = (BIT_ARRAY_TYPE+"* {BITARRAY_ID} = ("+BIT_ARRAY_TYPE+"*) (calloc({NUM_LONGS}, sizeof("+BIT_ARRAY_TYPE+"));\n").format(**bitarrays[var_ID])
        
        code = tail + REPLACEMENT + head

        if var_ID >=2:
            pass

    #make sure that the variable is actually in the string, and that it is being used (ie x[1][2], or x[3][4] = x[1][3])
    #def contains_variable_in_use(line, varstr):
    #    if varstr in line:
    #        line.index(varstr)
    #UNFINISHED AND UNUSED FOR NOW        

    #returns what is inbetween two sets of brackets bitA[...x...][...y...]
    def interpret_brackets(line, variable_name='', start=-1):
        """from var[a][b](awdas), returns "a", "b", and the index of "]("""""
        m = 0
        if start != -1:
            start_bracket = start
        elif variable_name:
            start_bracket = line.index("[",line.index(variable_name))
        else: print("INTERPRET_BRACKETS IN BITARRAY.PY WAS CALLED WITHOUT A START OR VARIABLE NAME")
        middle_bracket, end_bracket = 0,0
        i = start_bracket
        for char in line[start_bracket:]:
            i= i+1
            if char == "[":
                m = m+1
                continue
            if char == "]":
                m = m-1
                if m==0:
                    if not middle_bracket:
                        middle_bracket=i
                    elif not end_bracket:
                        end_bracket=i
                        break
                else:
                    continue

        if (not end_bracket) or (not middle_bracket):
            print("ERROR COUNTING BRACKETS IN A LINE INDEXING BITARRAYS")

        return line[start_bracket+1:middle_bracket-1], line[middle_bracket+1:end_bracket-1], end_bracket


    #generates the C code which attempts to replace a bitarray lookup (only replaces the first instance)
    def replace_get(variable_name, line):
        pos = 0
        for i in range(line.count(variable_name)):
            pos = line.index(variable_name, pos)
            if line[pos-2:pos+len(variable_name)] == "__"+variable_name:
                pos=pos+1

        x,y,_ = interpret_brackets(line, start=pos+1)    
        return line[0:pos]+ "get__{BITARRAY_ID}({x},{y})".format(**{"BITARRAY_ID":variable_name,"x":x,"y":y}) + line[_::]

    #generates the C code which attempts to replace a bitarray assignment (only replaces the first instance)
    def replace_set(variable_name, line):
        x,y, end_of_reference = interpret_brackets(line, variable_name)
        #print("IN SET")
        value = line[line.index("=",end_of_reference)+1:]

        return "set__{BITARRAY_ID}({x},{y},{value})".format(**{"BITARRAY_ID":variable_name,"x":x,"y":y,"value":value})

    #split the code into cleaned functional lines (we are "sloppy-fixing" the problem of curly brackets not always ending in semicolons)
    code = list(map(lambda x: x.strip(), code.replace("{","%OPENCURLY%").replace("}","%CLOSECURLY%;").split(sep=";")))

    #in this loop, we replace all references to the bitarray variable with proper C.
    for variable in list(bitarrays.items()):
        variable = variable[-1]
        variable_name = variable["BITARRAY_ID"]

        #look at each line which contains the variable
        for line in list(filter(lambda line: variable_name in line, code)):
            new_line = line;
            #determine if the line "properly" uses the variable (a loose definition of "proper")
            if (line.count("[") == line.count("]")) and (line.count("]") >= 2):
                #new_line = line.replace("\t"," ").replace("\n"," ").replace("   "," ").replace("  "," ")

                #make sure the variable usage is the type we are looking for
                if new_line[new_line.index(variable_name)+len(variable_name)] != "[":
                    continue
                
                #determine if we will be setting or getting a bitarray value
                new_line = new_line.replace("==","%EQUALEQUAL%")#if we have ==, then we MUST be getting where we may otherwise detect setting, so temporarily replace it with a marker
                

                #we _must be_ "get"ting, as there is no equal sign in this line
                if new_line.count("=") == 0:
                    for x in range(new_line.replace("__"+variable_name+"(","").count(variable_name)):
                        if new_line.count(variable_name)>new_line.count("__"+variable_name):
                            new_line = replace_get(variable_name, new_line)


                #we may be getting AND/OR setting
                elif new_line.count("=") == 1:
                    equals = new_line.index("=")
                    #address the variable occurence before the "=" sign (CURRENTLY only set, but could be get)
                    if new_line[0:equals].count(variable_name):
                        new_line = replace_set(variable_name, new_line)                    

                    #address the variable occurence after the "=" sign (will always be a get):
                    for x in range(new_line.replace("__"+variable_name+"(","").count(variable_name)):
                        if new_line[equals:].count(variable_name)>new_line[equals:].count("__"+variable_name):
                            new_line = replace_get(variable_name, new_line)
                        
                elif len(split_line) > 2:
                    print("ERROR - WHAT ARE YOU DOING WITH 3+ EQUAL SIGNS IN A LINE?!?\n Seriously, let Jake know, he's curious.\n")


                new_line = new_line.replace("%EQUALEQUAL%","==")
                code[code.index(line)]=new_line
                
    #at this point, we have fully functional C code.
    CODE = ';\n'.join(code).replace("%OPENCURLY%","{").replace("%CLOSECURLY%;","}")

    FINAL_DYNAMIC_CODE = COMMON_CODE_FOR_INSERTION
    for x in bitarrays:
        FINAL_DYNAMIC_CODE = FINAL_DYNAMIC_CODE + DYNAMIC_CODE_FOR_INSERTION.format(**bitarrays[x])

    FINAL_CODE = FINAL_DYNAMIC_CODE + CODE 

    with open(outfile, mode="w") as outfile:
        outfile.write(FINAL_CODE)
        outfile.close()
