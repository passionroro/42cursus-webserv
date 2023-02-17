# On branch roro

* Connection TCP serve/client established

> Note: put it in a while loop

* The server receives a request and prints it :
```c++
recv(client, buf, BUFFER_SIZE, 0);
```

* The server opens `index.html` :
```c++
std::ifstream       ifs;
ifs.open("home/www/index.html", std::ifstream::in);
if (!ifs.is_open())
    error("open()");
```

I used `istreambuf_iterator` (see alternatives [here](https://www.delftstack.com/howto/cpp/read-file-into-string-cpp/)) 
to transform a file stream into a string.
```c++
std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
```

If the file exists, a response is inserted at the beginning
of the file : 
```c++
str.insert(0, "HTTP/1.1 200 OK\\r\\n\\r\\n");
```

Finally, the response is sent and the client receives the page `index.html`
```c++
send(client, str.c_str(), str.length(), 0);
```

<333