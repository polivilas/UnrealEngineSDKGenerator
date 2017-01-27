# How to generate a new SDK?
To generate a new SDK you need to follow these steps.

## 1. Choose the engine version your game uses
To make your life easier, you should copy the files from a game which uses the same version of UE as the new game because you need to make less code changes.

Shipped games to choose from are:

	Unreal Engine 1:
		Unreal
	
	Unreal Engine 2:
		Unreal 2
		Unreal Tournament 2004
	
	Unreal Engine 3:
		All Points Bulletin: Reloaded
		Hawken
		Tribes: Ascends
		Unreal Tournament 3
	
	Unreal Engine 4:
		Alone in the Dark: Illumination
		ARK Survival Evolved
		Paragon
		Unreal Tournament 4

## 2. Generate new project files
If you have choosen a game to copy from just run the _CreateNewProject.bat_ file. This will run a powershell script which will ask you for the name of the project to copy from and the name of the new project.

**Example**:
You want to generate a SDK for a game called MyCoolGame. MyCoolGame uses the Unreal Engine 3 so you can then use Tribes: Ascends as template project. To create the new project files you execute the _CreateNewProject.bat_ file and enter 

	> copy from: TribesAscend
	> new name: MyCoolGame

(Hint: If the batch file is in the same folder as the other project files you can use TAB to autocomplete the names. Just remove the file extension in the name.)

The batch file copies the content, replaces some text and creates the new project files for you. You can do this manual too but why should you...? :smile:

As last step you need to open the _UnrealEngineSDKGenerator.sln_ and add the new _MyCoolGame.vcxproj_ to the solution.

### List of generated files
Each game has 5 files you may need to change.

_EngineClasses.hpp_
This file contains the classes which are needed to generate the SDK. The classes are game dependend and usually you need to make changes here.
_Generator.cpp_
This file contains settings which control the SDK generation. The classes are game dependen and usually you need to make changes here.
_GenericTypes.cpp_
This file contains wrapper classes which map the classes from _EngineClasses.hpp_ to an interface which allows the generator to work with every version of Unreal Engine without making changes everywhere.
Normaly you don't need to make changes here.
_NamesStore.cpp_
This file contains the game dependend access to the names used by the game. Here you need to provide access to the global names array used by Unreal Engine. With the pattern library you can use signatures to automaticly find the correct adress at runtime.
_ObjectsStore.cpp_
Same as the _NamesStore.cpp_ but with code for the objects array used by Unreal Engine.

## 3. Reverse engineer the needed classes
To get the classes you need to insert into _EngineClasses.hpp_ I recommend this video https://www.youtube.com/watch?v=yNe4jz1v0R4 from The Feckless.
It shows how you can find the name array (put the signature in _NamesStore.cpp_), the object array (put the signature in _ObjectsStore.cpp_) and the needed classes (put them in _EngineClasses.hpp_).
Again: To make your life easier, you should use the same member names in the classes so you don't need to change other places too.

## 4. Implement the project specific code
The _Generator.cpp_ file contains settings which control the SDK generation.

### First the easy methods:

`GetOutputDirectory()`
This method gets the path where the files should be generated. You don't need to add the game name because the name will get automaticly appended.
If you specify an absolute path (ex. "C:/SDK_GEN") the files will get generated there.
If you specify an relative path (ex. "SDK_GEN") the files will get generated in the directory where compiled dll is injected from appended with the relative path.
**Example**:
The dll gets injected from "C:/UnrealEngineSDKGenerator/bin/MyCoolGame.dll". So the output directory will be "C:/UnrealEngineSDKGenerator/bin/SDK_GEN".
The default value is "C:/SDK_GEN".

`GetGameName()`
This method should return the full name of the game. (ex. "MyCoolGame")

`GetGameNameShort()`
This method should return an abbreviation for the full name. The generated files get prefixed with this short name. (ex. "MCG")

`GetGameVersion()`
This method should return the current version of the game. The version number gets printed to the header of every file. (ex. "1.2.3.4")

`ShouldDumpArrays()`
If this method returns true (default) the SDK dumper generates two textfiles which contain a list of all names and the names of the objects.

`ShouldGenerateEmptyFiles()`
If this method returns false (default) no package files are generated when the package doesn't contain classes, constants or enums.

`ShouldUseStrings()`
If this method returns true (default) the objects are referenced by their name. Otherwise the objects global index will be used.
Warning: The index may change on updates or even on every start of the games.

`ShouldXorStrings()`
If this method returns true (default: false) the strings printed by the generator get surrounded by `_xor_(...)`. With the XorStr library these strings get xor encrypted at compile time.
https://svn.oldschoolhack.me/listing.php?repname=XorStr+%28...%2Fxorstr%29

`ShouldConvertStaticMethods()`
If this method returns true (default) the static methods of a class are converted to normal methods. Static methods are only syntactic sugar from the engine. They are normal methods but called with the default object of a class. From the user point of view they are static methods but in reality they are not. If you want to use static methods you need to provide a method called `CreateDefaultObject()` in the UObject class. Some of the projects have signatures for the engine method but they could be hard to find. `ShouldConvertStaticMethods()` allows to rewrite the static methods to work like normal methods which require a class instance to get called. Additionaly these methods get a "STATIC_" prefix.

`GetNamespaceName()`
To seperated the generated classes from the rest of your project you can surround the classes with a namespace. By default no namespace is generated.
This method should return the name of the namespace. If an empty name is given no namespace will be generated. (ex. "Classes")
The generated output looks like:

```cpp
namespace Classes
{
	...
}
```

`GetIncludes()`
If your code needs some special header files to work return them in this method. For example you could need a pattern search library so add the related header file.
**Example**:

```cpp
return {
	R"("../my/relative/path/patternscan.hpp")", //with the raw literal you don't need to escape the quotes
	"\"../math/vector.hpp\"",
	"<windows.h>"
};
```

`GetGlobalMemberAlignment()`
This method returns the alignment of members. By default this method returns 4 at 32bit and 8 at 64bit.
See https://msdn.microsoft.com/en-us/library/2e70t5y1.aspx for more information.

### The following methods are just getters and should not be modified:

`GetOverrideType()`
This method checks if you have specified an override type for the related type. If no override type is found the original type is returned. This is usefull if you want to map some of your classes into the SDK. For example you could replace the provided `FVector` class with a version of your choice. You need to make sure the replacement hast the same size as the original class. If your override type is declared in an other header file you should add the include with the `GetIncludes()` method.

`GetPredefinedClassMembers()`
For some classes the dumper can't generate members. If a class needs to have meaningfull members (for example the important `UObject` class) you can specify the members and this class will return them if the classname is matched.

`GetPredefinedClassStaticMembers()`
Sometimes classes should have static members. This methods returns them if you have specified them.

`GetVirtualFunctionPatterns()`
If you need virtual methods in a class you can specify patterns to match these methods. The generator loops the virtual methods of the class and checks for pattern matches. If a pattern matches the specific method will be added to the class.

`GetPredefinedClassMethods()`
If you want to add methods to a class you can specify them and this method returns them.

`GetClassAlignas()`
Some classes need a special memory alignment (since Unreal Engine 4). This method gets the alignas value for the classes you have specified.

### And now the advanced methods: :wink:

`GetBasicDeclarations()`
This method returns a string with code which contains all the basic classes. Usually this is the same code you need to put in the _EngineClasses.hpp_. You should check the contained projects for examples.

`GetBasicDefinitions()`
This method returns a string with code which contains definitions for the classes defined by the `GetBasicDeclarations()` method. You should check the contained projects for examples.

`Initialize()`
This is the main method for you! This method initializes all the stuff the other methods need to work. You can do anything here (disable a possible protection, ...) but usually you just need to provide some data the getter methods can return.
It looks complicated but for the best explanation of this method you should have a look at the provided projects.

*	`overrideTypes`
	This map contains the override classes used by `GetOverrideType()`.

	**Example**:
	This example forces the generator to override both `FVector` classes with the custom classes.
	```cpp
	overrideTypes = {
		{ "struct FVector", "class Vector3D" },
		{ "struct FVector2D", "class Vector2D" }
	};
	```
*	`alignasClasses`
	This map contains the alignas classes used by `GetClassAlignas()`.

	**Example**:
	This example forces the alignas keyword for the three listed classes.
	```cpp
	alignasClasses = {
		{ "ScriptStruct CoreUObject.Plane", 16 },
		{ "ScriptStruct CoreUObject.Vector4", 16 },
		{ "ScriptStruct Engine.RootMotionSourceGroup", 8 }
	};
	```

*	`virtualFunctionPattern`
	This map is used by `GetVirtualFunctionPatterns()` and contains signatures which are used to find virtual methods in classes.

	**Example**:
	This example adds a signature which if found adds the ProcessEvent method to the Object class.
	```cpp
	virtualFunctionPattern["Class CoreUObject.Object"] = {
		{
			"\x45\x33\xF6\x4D\x8B\xE0",
			"xxxxxx",
			R"(inline void ProcessEvent(class UFunction* function, void* parms)
			{
				return GetVFunction<void(*)(UObject*, class UFunction*, void*)>(this, %d)(this, function, parms);
			})"
		}
	};
	```

*	`predefinedMembers`
	This map is used by `GetPredefinedClassMembers()` and contains member variables which can't be dumped. At the moment you can't mix auto generated and predefined members.
	The members are added in the class in the same order as in the map.

	**Example**:
	This example adds the listed members to the Object class.
	```cpp
	predefinedMembers["Class CoreUObject.Object"] = {
		{ "void*", "Vtable" },
		{ "int32_t", "ObjectFlags" },
		{ "int32_t", "InternalIndex" },
		{ "class UClass*", "Class" },
		{ "FName", "Name" },
		{ "class UObject*", "Outer" }
	};
	```

*	`predefinedStaticMembers`
	This map is used by `GetPredefinedClassStaticMembers()` and contains static member variables.

	**Example**:
	This example adds the static member GObjects with the type FUObjectArray* to the Object class.
	```cpp
	predefinedStaticMembers["Class CoreUObject.Object"] = {
		{ "FUObjectArray*", "GObjects" }
	};
	```

*	`predefinedMethods`
	This map is used by `GetPredefinedClassMethods()` and contains custom methods. This can be used to add helper functions like constructors.

	**Example**:
	This example adds two constructors to the Vector2D class to make initialization easier.
	```cpp
	predefinedMethods["ScriptStruct CoreUObject.Vector2D"] = {
		PredefinedMethod::Inline(R"(inline FVector2D() : X(0), Y(0) { })"),
		PredefinedMethod::Inline(R"(inline FVector2D(float x, float y) : X(x), Y(y) { })")
	};
	```

## And now?
Compile the project and inject the DLL into the target. Use the debug build and a debugger to fix errors but use the release build to really generate the sdk. Otherwise you need to wait some minutes because the debug build is very slow. After you see the "Finished!" messagebox you can have a look at your new sdk.

The generated folder structure looks like this:
```
XXX
+-- Generator.log
+-- NamesDump.txt
+-- ObjectsDump.txt
+-- SDK.hpp
+-- SDK
|   +-- XXX_Basic.hpp
|   +-- XXX_Basic.cpp
|   +-- XXX_..._classes.hpp
|   +-- XXX_..._functions.cpp
|   +-- XXX_..._structs.hpp
```

_Generator.log_
This file contains the log messages the generator outputs.
_NamesDump.txt_
This file is generated if `ShouldDumpArrays()` is true and it contains all names available in the names array.
_ObjectsDump.txt_
This file is generated if `ShouldDumpArrays()` is true and it contains all objects names available in the objects array.
_SDK.hpp_
This file contains all includes you need for the SDK.

*XXX_Basic.hpp* / *XXX_Basic.cpp*
These files contain the code outputted by `GetBasicDeclarations()` and `GetBasicDefinitions()`.
*XXX_..._classes.hpp* / *XXX_..._structs.hpp*
These files contain the code of the classes and structs.
*XXX_..._functions.cpp*
This file contains the code of the class methods.

The SDK folder contains the generated code for every package. You need to copy the _SDK.hpp_ and the _SDK_ folder to your project. In your code you need an include to the _SDK.hpp_ and add the _*.cpp_ files to the project. Most of the time you don't need all the cpp files.
Files you may always need:
```
XXX_Basic.cpp
XXX_Core(UObject)_functions.cpp
XXX_Engine_functions.cpp
```
To find these files just sort the SDK folder by file size. The biggest files are the files you are looking for.

Now you can use the SDK in your project. Have fun. :smile:

## Known Errors
Sometimes you can't compile the generated SDK. Here are some known errors and a solution how to fix them:

- (UE4) In the generated enum EPixelFormat (CoreUObject_classes.hpp) is a PF_MAX value. The windows.h header defines a constant with the same name so this will keep you from compiling. Just remove the PF_MAX value from enum and all should work.
- Sometimes class members or method parameters get a name like "return", "continue", "break" which are reserved keywords so the compiler will complain. To fix this error just rename these to something else.