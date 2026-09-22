#!/usr/bin/env python3
import os
import sys

env_html = []
for k, v in os.environ.items():
    env_html.append(f"<li>{k}={v}</li>")

env_section = "\n".join(env_html)

body = f"""<!DOCTYPE html>
<html>
<head><title>Status CGI Test</title></head>
<body>
    <h1>CGI Status Header Test</h1>
    <p>If you see this page, Status parsing works.</p>

    <h2>CGI Environment</h2>
    <ul>
        {env_section}
    </ul>
</body>
</html>
"""

# while(true)
body_bytes = body.encode("utf-8")

# CGI headers ONLY (no HTTP/1.1 line)
sys.stdout.write("Status: 201 Created\r\n")
sys.stdout.write("Content-Type: text/html; charset=utf-8\r\n")
sys.stdout.write(f"Content-Length: {len(body_bytes)}\r\n")
sys.stdout.write("\r\n")

# Body
sys.stdout.write(body)
sys.stdout.flush()
