{
  "targets": [
    
 	
    	
    {
      "target_name": "shellex",
     
      	      
      
      "include_dirs": [
		 'src/astox'
	  ],
	  
	 
	  
	  'defines': [
             'ASTX_ENABLE_NODEJS',
             'ASTX_DISABLE_EXCEPTIONS=1',
             'ASTX_VALUE_STATIC_CAST=1'
             'NETSNMP_ENABLE_IPV6'
	  ],
	
	  "sources": [
		"src/shellex.cc",
		
		#common libs:
		"src/astox/StringAlg.cpp",
		"src/astox/FileSystem.cpp",
		"src/astox/Socket.cpp",
		"src/astox/FileSystem.cpp",
		"src/astox/ExtApi.cpp",
		"src/astox/Thread.cpp",
		"src/astox/EventDispatcher.cpp",
		
		#astox objects:
		"src/astox/Value.cpp",
		"src/astox/String.cpp",
		"src/astox/Number.cpp",
		"src/astox/Boolean.cpp",
		"src/astox/Array.cpp",
		"src/astox/Object.cpp",
		"src/astox/Function.cpp",
		"src/astox/Pins.cpp",
		"src/astox/Operators.cpp",
		"src/astox/Lang.cpp",
		"src/astox/Date.cpp",
		"src/astox/ECMAScript.cpp",
		"src/astox/EcmaMethods.cpp",
		
		#extensions:
		"src/astox/Musteen.cpp",
		"src/astox/CssLinter.cpp"
	  ]

      
    }
  ]
}