{
  "targets": [
    {
      "target_name": "bitscope",
      "sources": [
        "bitscope.cc",
        "init.cc",
        "setup.cc",
        "acquire.cc",
        "trace.cc",
        "close.cc"
      ],
      "include_dirs": ["<!(node -e \"require('nan')\")"],
      "libraries": [
        "-lBitLib"
      ]
    }
  ]
}
