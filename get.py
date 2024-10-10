import requests

import json

get_url='http://localhost:3000/problem?token=a'

header={"Content-Type":"application/json"}

response=requests.get(get_url,{'key':'value'},)


file = open("./data.json","wb")

for chunk in response.iter_content(100000):
  file.write(chunk)

file.close()