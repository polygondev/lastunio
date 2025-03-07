import requests
import serial
import time

API_KEY = "[LASTFM API KEY]"
USERNAME = "[LASTFM USERNAME]"
LASTFM_URL = f"http://ws.audioscrobbler.com/2.0/?method=user.getrecenttracks&user={USERNAME}&api_key={API_KEY}&format=json"

SERIAL_PORT = "COM3"  # Change to serial port
BAUD_RATE = 9600

ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
time.sleep(2)

def get_now_playing():
    """Fetch currently scrobbling song from Last.fm"""
    response = requests.get(LASTFM_URL)
    data = response.json()

    if "recenttracks" in data and "track" in data["recenttracks"]:
        tracks = data["recenttracks"]["track"]
        if len(tracks) > 0 and "@attr" in tracks[0] and "nowplaying" in tracks[0]["@attr"]:
            song = tracks[0]["name"]
            artist = tracks[0]["artist"]["#text"]
            album = tracks[0]["album"]["#text"]
            return f"{song}|{artist}|{album}"
    return "No song|No artist|No album"

while True:
    now_playing = get_now_playing()
    print(f"Sending: {now_playing}")
    ser.write(now_playing.encode("utf-8"))
    time.sleep(10)

