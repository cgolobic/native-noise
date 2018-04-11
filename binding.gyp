{
  "targets": [
    {
      "target_name": "noise",
      "sources": ["noise.cc"],
      "include_dirs" : [
          "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}