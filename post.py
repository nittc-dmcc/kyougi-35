import requests

import json

post_url='http://localhost:3000/answer?token=a'

header = {"Content-Type":"application/json"}


json_open = open("post.json","r")

post_dict = json.load(json_open)

post_encode = json.dumps(post_dict)

r = requests.post(url=post_url, headers=header, data=post_encode)
print(r.status_code)
print(r.text)