{
    "targets": [{
        "target_name": "biometric",
		"sources": [ 
            "src/new/verify.cpp",  
            "src/new/base64.cpp", 
            "src/new/main.cpp",
            "src/new/capture.cpp",
            "src/new/selection.cpp", 
            "src/new/helpers.cpp",
            "Include/dpfj_compression.h",
            "Include/dpfj_quality.h",
            "Include/dpfj.h",
            "Include/dpfpdd.h"],
        "conditions": [
            ['OS=="linux"',{   
                "include_dirs": [
                    "<!(node -e \"require('nan')\")", 
                    "Include",
                    "lib"
                ],
                "libraries": [
                    "<(module_root_dir)/lib/*",
                ]
            }], 
            ['OS=="win"',{                
                "include_dirs": [
                    "<!(node -e \"require('nan')\")",
                    "<!(node -e \"require('zlib')\")", 
                    "./Include",
                    "./lib"
                ],         
                "libraries": [
                    "<!(node -e \"require('zlib')\")",  
                    "<(module_root_dir)/lib/*",
                ]
            }]
        ],
        "variables": {
            "node_version": '<!(node --version | sed -e "s/^v\([0-9]*\\.[0-9]*\).*$/\\1/")',
        },
        "target_conditions": [
            [ "node_version >= '0.10'", { "defines": ["OLD_UV_RUN_SIGNATURE"] } ]
        ]
    }]
}