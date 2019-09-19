import csv
import requests
from urllib.parse import urlparse
from urllib.parse import parse_qs

f = open('images.csv')
csv_f = csv.reader(f)
for row in csv_f:
	url = row[0]
	newfile = row[1]
	if not newfile.endswith('jpg'):
		newfile+='.jpg'
	parse = urlparse(url)
	if parse.scheme != '':
		query =  parse_qs(parse.query)
		if ('id' in query) and (parse.path == '/open'):
			url = 'https://drive.google.com/uc?authuser=0&id=' + query['id'][0] +'&export=download'
		print ('...downloading...' + url)
		r = requests.get(url,allow_redirects=True)
		open(newfile,'wb').write(r.content)