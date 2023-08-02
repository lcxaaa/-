CREATE DATABASE work;

use work;

create table t_friend(
	idA VARCHAR(10) ,
    idB VARCHAR(10) 
);

create index friendIndex on t_friend(idA,idB);

create table t_user(
	userid varchar(20)  primary key,
    password varchar(40)  not null,
    status int default 0
);

create table t_house(
	housename varchar(10) ,
    hostname varchar(10) ,
    username varchar(10)  ,
    AImode int default 0
);
create index houseIndex on t_house(username);

 create table t_score( 
 userid varchar(20) primary key,
 times int default 0,
 win int default 0
 );
 
  create table t_game(
  idA varchar(20) unique not null,
  idB varchar(20) unique not null ,
  win varchar(20) unique not null ,
<<<<<<< HEAD
  Time datetime not null,
  EndTime datetime not null
=======
  Time datetime not null
>>>>>>> 9b8b59bd73e9cb2f463f9a11963fa51210cdbd1d
  );