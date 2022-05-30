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
            "src/new/scann.cpp",
            "Include/dpfj_compression.h",
            "Include/dpfj_quality.h",
            "Include/dpfj.h",
            "Include/dpfpdd.h"],
        "conditions": [
            ['OS=="linux"',{   
                "copies": [{
                    "destination": "<(module_root_dir)/build/Release",
                    "files": [
                        "<(module_root_dir)/Include",
                        "<(module_root_dir)/lib",
                    ]
                }],
                "include_dirs": [
                    "<!(node -e \"require('nan')\")",  
                    "<(module_root_dir)/Include", 
                ],
                "libraries": [ 
                    "<(module_root_dir)/lib/libdpfpdd.so",
                    "<(module_root_dir)/lib/libdpfj.so",
                    "<(module_root_dir)/lib/libtfm.so",
                    "<(module_root_dir)/lib/libWSQ_library64.so",
                ],
                "cflags!": [ "-fno-exceptions" ],
                "cflags_cc!": [ "-fno-exceptions" ],
            }], 
            ['OS=="win"',{ 
                "copies": [{
                    "destination": "<(module_root_dir)/build/Release",
                    "files": [
                        "<(module_root_dir)/Include",
                        "<(module_root_dir)/lib",
                    ]
                }],             
                "include_dirs": [
                    "<!(node -e \"require('nan')\")",
                    "<!(node -e \"require('zlib')\")",
                    "<(module_root_dir)/Include", 
                ],         
                "libraries": [
                    "<!(node -e \"require('zlib')\")",
                    "<(module_root_dir)/lib/libdpfpdd.so",
                    "<(module_root_dir)/lib/libdpfj.so",
                    "<(module_root_dir)/lib/libtfm.so",
                    "<(module_root_dir)/lib/libWSQ_library64.so",
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