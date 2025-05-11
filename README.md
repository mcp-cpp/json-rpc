# json_rpc

[中文](README_zh.md)

Pure and lightweight Modern C++ [JSON-RPC 2.0](http://www.jsonrpc.org/specification) library

## Component

Jsonrpc implements [JSON-RPC 2.0](https://www.jsonrpc.org/specification) all objects:

* [Request](http://www.jsonrpc.org/specification#request_object)
    * [Notification](http://www.jsonrpc.org/specification#notification)
    * [Parameter](http://www.jsonrpc.org/specification#parameter_structures)
* [Response](http://www.jsonrpc.org/specification#response_object)
    * [Error](http://www.jsonrpc.org/specification#error_object)
* [Batch](http://www.jsonrpc.org/specification#batch)

Jsonrpc is based on [JSON for Modern C++](https://nlohmann.github.io/json/).

## Code example

```c++
  std::string json_str = R"({
        "jsonrpc": "2.0",
        "method": "subtract",
        "params": [42, 23],
        "id": 1
    })";
    
   Request request;
   if (!request.ParseJson(json_str)) {
     // invalid request
     return;
   }
   if (request.IsNotification()) {
     // no response needs to be returned
     return; 
   }
   if (request.IsInternalMethod()) {
     // deal with internal method
     return;
   }
   Response response(request.Id());
   if (request.Method() == "subtract") {
       int a = request.Params().Array()[0].get<int>();
       int b = request.Params().Array()[1].get<int>();
       response.SetResult(a - b);
   }
```

[More code example](json_rpc/unit_test/examples.cc)