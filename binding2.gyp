{
    "targets": [{
        "target_name": "biometric",
		"sources": [ 
            "src/new/verify.cpp",  
            "src/new/base64.cpp", 
            "src/new/main.cpp",
            "src/new/capture.cpp",
            "src/new/selection.cpp", 
            "src/new/helpers.cpp"],
        "conditions": [
            ['OS=="linux"',{
                "copies": [{
                    "destination": "<(module_root_dir)/build/Release/",
                    "files": [
                        "<(module_root_dir)/Include/dpfpdd.h",
                        "<(module_root_dir)/Include/dpfj.h",
                        "<(module_root_dir)/Include/dpfj_quality.h",
                        "<(module_root_dir)/Include/dpfj_compression.h",
                        "<(module_root_dir)/lib",
                    ]
                }],
                "include_dirs": [
                    "<!(node -e \"require('nan')\")",
                    "<(module_root_dir)/Include",
                    "<(module_root_dir)/lib",
                ],
                "libraries": [
                    "-Llib",
                    "-L$ORIGIN/build/Release/Include/dpfpdd.h",
                    "-L$ORIGIN/build/Release/Include/dpfj.h",
                    "-L$ORIGIN/build/Release/Include/dpfj_quality.h",
                    "-L$ORIGIN/build/Release/Include/dpfj_compression.h",
                    "-Wl,-rpath,$ORIGIN/build/Release/lib/libdpfpdd.so",
                    "-Wl,-rpath,$ORIGIN/build/Release/lib/libdpfj.so",
                    "-Wl,-rpath,$ORIGIN/build/Release/lib/libtfm.so",
                    "-Wl,-rpath,$ORIGIN/build/Release/lib/libWSQ_library64.so",
                    "-Wl,-rpath,$ORIGIN/build/Release/lib/libdpfpdd_4k.so.5.0.0",
                    "-Wl,-rpath,$ORIGIN/build/Release/lib/libdpfpdd_ptapi.so.1.0.0",
                    "-Wl,-rpath,$ORIGIN/build/Release/lib/libdpfpdd5000.so.3.1.1",
                    "-Wl,-rpath,$ORIGIN/build/Release/lib/libdpfr6.so.3.0.0",
                    "-Wl,-rpath,$ORIGIN/build/Release/lib/libdpfr7.so.3.0.0",
                    "-Wl,-rpath,$ORIGIN/build/Release/lib/libdpuareu_jni.so.3.0.0"
                ],
                "link_settings": {
                    "libraries": [
                        "-Wl,-rpath,-Wl,-rpath,$ORIGIN/build/Release/lib/dpfpdd.h",
                        "-Wl,-rpath,-Wl,-rpath,$ORIGIN/build/Release/lib/dpfj.h",
                        "-Wl,-rpath,-Wl,-rpath,$ORIGIN/build/Release/lib/dpfj_quality.h",
                        "-Wl,-rpath,-Wl,-rpath,$ORIGIN/build/Release/lib/dpfj_compression.h",
                        "-Wl,-rpath,-Wl,-rpath,$ORIGIN/build/Release/Include/"
                    ],
                }
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