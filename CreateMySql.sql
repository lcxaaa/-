CREATE DATABASE work;

use work;

create table t_friend(
	idA VARCHAR(10) ,
    idB VARCHAR(10) 
);

create table t_user(
	userid varchar(20) ,
    passwork varchar(40) ,
    status int default 0
);

create table t_house(
	housename varchar(10) ,
    hostname varchar(10) ,
    username varchar(10)  ,
    AImode int default 0
);