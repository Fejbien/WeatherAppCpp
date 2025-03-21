import json
import requests

url = 'https://api.gios.gov.pl/pjp-api/rest/station/findAll'

response = requests.get(url)
data = response.json()

filtered_data = []

for station in data:
    filtered_data.append({
        'id': station['id'],
        'city': station['city']['commune']['communeName'],
        'district': station['city']['commune']['districtName'],
        'province': station['city']['commune']['provinceName'],
        'station_street': station['stationName'],
    })

with open('citySearchData.json', 'w', encoding='utf-8') as json_file:
    json.dump(filtered_data, json_file, ensure_ascii=False, indent=4)