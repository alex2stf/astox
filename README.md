# astox
lightweight scripting engine



## Default scripting engine:
#### accepted differences between astox and javascript:
<table>
<tr> 
<td>astox</td>
<td>ecma</td>
</tr>
<tr> 
<td>
instanceof really check if the value provided is an instance of the parameter string given by checking the instance, not the constructor:
</td>
<td>
tests whether an object has in its prototype chain the prototype property of a constructor
</td>
</tr>
</table>
```javascript
var a = "some string";
print(a instanceof string); 
// will output "some string" instead of true

//to assign value of instanceof:
var answ = (a instanceof string);
print(answ); // will output "true". In javascript will return "false";

//instanceof arguments are not case-sensitive:
var answ = (a instanceof String); 
print(answ); // will output "true". In javascript will return "false";

var answ = (a instanceof StRinG); 
print(answ); // will still output "true".
```



<table>
<tr> 
<td>astox</td>
<td>ecma</td>
</tr>

<tr> 
<td>
regex supported only as method argument:
</td>
<td>
inline regex object declaration:
</td>
</tr>
</table>


```javascript
var a = RegEx(/search/);
print(a); 
// will output "search" instead of "/search/"
a = /search/; // will produce errors
```

Date object:
for values after 1970 unix timestamp will be returned (same as js)
for values from 0 to 1970 js values will be returned
for values before 0 some differences were found in unit-testing



## Building astox:
### Linux / Mac:
#### optional linux enviroment flags for main Makefile:
<table>
    <tr> 
        <td>name</td>
        <td>value</td>
        <td>details</td>
    </tr>
    
    <tr> 
        <td>ASTX_ENABLE_CURL</td>
        <td>export ASTX_ENABLE_CURL=true</td>
        <td>support for curl methods</td>
    </tr>
    
    
     <tr> 
        <td>ASTX_ENABLE_SSH2</td>
         <td>export ASTX_ENABLE_SSH2=true</td>
        <td>support for ssh2 methods (scp and sftp)</td>
     </tr>
     
     
      <tr> 
        <td>ASTX_ENABLE_SASS</td>
        <td>export ASTX_ENABLE_SASS=true</td>
        <td>support for sassc compiler options</td>
     </tr>
     
     
      <tr> 
        <td>ASTX_ENABLE_MINGUI</td>
        <td>export ASTX_ENABLE_MINGUI=true</td>
        <td>support for minimal gui components</td>
     </tr>
</table>
all libs should exist in PATH enviroment

#### customize main Makefile:
you are free to add your custom libs/includes files by appending info to following existing variable:
```bash
    LIBS :=$(LIBS) -lMyLib
    LIB_PATHS :=$(LIB_PATHS) -L/my/path/to/my/library
    DEFINES :=$(DEFINES) -DMY_CUSTOM_DEFINE_FLAG
    INCLUDES :=$(INCLUDES)-I/my/custom/include/folder
    OTHER_FLAGS :=$(OTHER_FLAGS) -my -optional -flags
```
keep in mind that option "-ldl" already used my ASTX_ENABLE_SASS should be the last

### SelfConfigure tool:
running Makefile with/without any enviroment flags will generate astxSelfConfigure into the "bin folder"

<br />
> (markdown tested with grip tool)