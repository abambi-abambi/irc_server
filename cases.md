### Check active ports:
sudo lsof -nP -i | grep LISTEN

Check commands:

### PASS - check wrong cases : + nc & KVirc

pas
pass
pass 3

<!-- При наборе команды NICK _до_ прохождения регистрации пароля KVIrc попытается дать свой ник -->
nick n1

user q1 0 * sdfasdf
join #1
privmsg #1 message_text
notice #1 message_text
kick a3
pass

quit


### NICK - check wrong cases : + nc & KVirc

nick
nick !ede$~fs
nick 12345678901234567890
nick !1`fsdfad

pass 3
user q1 0 * adasd
join #1
privmsg #1 sdfsaf
kick a3 OK
nick n1
nick n6
nick n1

### Change NICK - check wrong cases : + nc & KVirc

+ before command NICK:
:n3 NICK n5

+ after NICK:
:n998 NICK n7
<!-- = wrong first param (should be equal to the old nick) -->

### USER - check wrong cases : + nc & KVirc

user
join #1
user u1 0 d sdafasdf
user u1 f * sdafasdf
user u1 0 * sdafasdf
user u1 0 * sdafasdf

### JOIN - check wrong cases : + nc & KVirc

join
join 1
join #
join #4,
join #4 ,
join #12345678901234567890
join #2
join #3
join #4# текстоповещение

### QUIT - check wrong cases : + nc & KVirc

quit 
quit text_message

(before pass
after pass
after nick
after user
after join)

### KICK - check wrong cases : + nc & KVirc

* before pass: +
kick
kick n1
kick #1 n1

* after pass: +
kick
kick n1
kick #1 n1

* after nick: +
kick
kick n1
kick #1 n1

* after user: +
kick
// no such channel:
kick #99
// you are not in that channel:
kick #2
// you are not channel operator:
kick #2

* after join:
kick #99 n2
// you are not in that channel:
kick #2 n2
// you are not channel operator:
kick #2 n2

// no such nick
kick #1 n2
// this user is not in that channel
kick #1 n2

kick #1 n2 
+

### PRIVMSG - check wrong cases : + nc & KVirc

* before pass: +
privmsg #1 sdfsaf
privmsg n1 sdfsaf

* after pass: +
privmsg #1 sdfsaf
privmsg n1 sdfsaf

* after nick: +
privmsg #1 sdfsaf
privmsg n1 sdfsaf

* after user: +
privmsg #1 sdfsaf
privmsg n1 sdfsaf 

* after join: +
privmsg
<!-- юзера нет, не хватает параметров -->
privmsg anonim
<!-- канала нет, не хватает параметров -->
privmsg #777
<!-- юзер есть, не хватает параметров -->
privmsg n1
<!-- канал есть, пользователь не в нем, не хватает параметров -->
privmsg #2
<!-- канал есть, пользователь в нем, не хватает параметров -->
privmsg #1

privmsg #99

* 3 params:
<!-- сообщение несуществующему юзеру -->
privmsg n99 text
<!-- сообщение в несуществующий канал -->
privmsg #99 text
<!-- канал существует, но юзер не в нем -->
privmsg #2 text
<!-- слишком много указано каналов (больше 1-го) == запятая невалидна -->
privmsg #2, #3 text
<!-- больше 1-го канала не рассматриваем, обрабатываем так: первый аргумент это канал, остальное - текст -->
<!-- нужен другой юзер в канале для получения им сообщения -->
privmsg #1 #2 text 
(текст видит в канале другой пользователь этого канала)
privmsg #1 #1
(текст видит в канале другой пользователь этого канала)

<!-- OK: -->
privmsg n1 text
privmsg #1 text

<!-- когда зареганный полностью юзер и при этом 
_уже_зашел в _любой_ канал,
он может писать любому юзеру напрямую,
даже если они оба находятся в _разных_ каналах:
privmsg n_user hello -->

Сообщения в KVirc:
<!-- 
- в розовом облаке: юзер-юзеру;
- иконка с решеткой: в канал. -->

KVirc: юзер-юзеру: +
<!-- - себе или другому юзеру в открывшейся странице диалога (т.е. внутри странички с облачком): пишем _только_ текст (если писать как по правилам privmsg n_user text это вообще не пройдет, исчезнет в никуда);
- с главной страницы (где логинились) пишем себе или другому юзеру через privmsg n_user text -->

KVirc: юзер-в-канал: +
<!-- - в канал в открывшейся странице канала (т.е. внутри странички с иконкой с решеткой): пишем _только_ текст (если писать как по правилам privmsg #channel text это вообще не пройдет, исчезнет в никуда);
- с главной страницы (где логинились) пишем себе или другому юзеру через privmsg #channel text -->

### NOTICE - check wrong cases : + nc & KVirc

same commands/cases as with PRIVMSG

### OUR's - check wrong cases : + nc & KVirc
usersall +
chansall +
usersinchan +
myname +
help +

### BONUS 1 - check wrong cases : + nc & KVirc
bot +

### BONUS 2 - check wrong cases : + nc & KVirc

Create 1-st KVirc client,
Create 2-nd KVirc client:

/DCC send your_nick
/DCC send n1 (for example)
(choose a file)

Try to send:
before pass: not allowed
after pass: not allowed
after nick: not allowed
after user: "Can't send text to channel ..."
after join: OK!

Try to receive:
after nick: OK!

отправитель: в канале +
принимающий: не в канале (pass+/nick+) +

НЕЛЬЗЯ: отправитель: не в канале (pass+/nick+)
НЕЛЬЗЯ: принимающий: в канале +

НЕЛЬЗЯ: быть отправителем, пока отправитель не вошел в любой канал
Зато можно быть получателем при этом (достаточно иметь ник)