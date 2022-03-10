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
                "copies": [{
                    "destination": "<(module_root_dir)/build/Release/lib",
                    "files": [
                        "<(module_root_dir)/lib/libdpfpdd.so",
                        "<(module_root_dir)/lib/libdpfj.so",
                        "<(module_root_dir)/lib/libtfm.so",
                        "<(module_root_dir)/lib/libWSQ_library64.so",
                        "<(module_root_dir)/lib/libdpfpdd_4k.so.5.0.0",
                        "<(module_root_dir)/lib/libdpfpdd_ptapi.so.1.0.0",
                        "<(module_root_dir)/lib/libdpfpdd5000.so.3.1.1",
                        "<(module_root_dir)/lib/libdpfr6.so.3.0.0",
                        "<(module_root_dir)/lib/libdpfr7.so.3.0.0",
                        "<(module_root_dir)/lib/libdpuareu_jni.so.3.0.0",
                        "<(module_root_dir)/lib/libdpuvc.so.3.1.1",
                    ]
                }],
                "include_dirs": [
                    "<!(node -e \"require('nan')\")", 
                    "<(module_root_dir)/Include",
                    "<(module_root_dir)/lib"
                ],
                "libraries": [
                    "-Llib",  
                    "-Wl,-rpath=./lib"
                ],
                "cflags!": [ "-fno-exceptions" ],
                "cflags_cc!": [ "-fno-exceptions" ],
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
                    "-l/Include",
                    "-l/lib"
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