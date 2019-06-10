{
    "targets": [{
        "target_name": "uareuprint",
		"sources": [ "src/main.cpp" , "src/identify.cpp" , "src/selection.cpp" , "src/helpers.cpp" ],
        "conditions": [
            ["OS in \"linux\"",{                
                "include_dirs": [
                    "<!(node -e \"require('nan')\")",
                    "<!(node -e \"require('zlib')\")", 
                    "/opt/Crossmatch/urusdk-linux/Include"
                ],
                "libraries": [
                "<!(node -e \"require('zlib')\")",  
                    "/usr/lib/libdpfpdd.so",
                    "/usr/lib/libdpfj.so",
                    "/usr/lib/libtfm.so",
                ]
            }], 
            ["OS in \"win\"",{                
                "include_dirs": [
                    "<!(node -e \"require('nan')\")",
                    "<!(node -e \"require('zlib')\")", 
                    "./Include",
                    "./lib"
                ],         
                "libraries": [
                    "<!(node -e \"require('zlib')\")",  
                    "-l/Include",
                    "-l/lib"
                ]
            }]
        ],
        "variables": {
            "node_version": '<!(node --version | sed -e "s/^v\([0-9]*\\.[0-9]*\).*$/\\1/")',
        },
        "target_conditions": [
            [ "node_version == '0.10'", { "defines": ["OLD_UV_RUN_SIGNATURE"] } ]
        ]
    }]
}