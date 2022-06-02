# FP_Kripto


- Server
- Client
  - Datagame yang disimpan client Health, Attack
  - Alur Game
    - Login -> data disimpan diserver.
      - Login
        - Matchmaking -> Waiting -> Ada notif ketemu print 3/2/1 -> Client Load Asset Game
        - View Leaderboard -> Ngambil dari Server
      - In Game
        - Ketik HIT untuk memukul musuh. Hit send supaya darah musuh berkurang - attack 
        - Kalau Salah satu darah player habis game berakhir dan musuh dapat pesan

- Server
  - Server Akun(Username,Password, Menang)


- Enkripsi
- 
### C : Login
"login username password"

### C : Matchmaking
"matchmake healthpoint attack"

### S : Send Enemy ID
"enemy|id|roomid"

### C : Ingame
"hit attack"

### S : Attacked
"damage|attacker|defender"

### S : Attacked
"gameover|win\lose"












