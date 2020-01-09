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
            ["OS in \"linux\"",{   
                "include_dirs": [
                    "<!(node -e \"require('nan')\")", 
                    "<(module_root_dir)/Include", 
                ],
                "libraries": [ 
                    "-L<(module_root_dir)/Include",
                    "-L<(module_root_dir)/lib",
                    "<(module_root_dir)/lib/libdpfpdd.so",
                    "<(module_root_dir)/lib/libdpfj.so",
                    "<(module_root_dir)/lib/libtfm.so",
                    "<(module_root_dir)/lib/libWSQ_library64.so"
                ]
            }], 
            ['OS in "win"',{                
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