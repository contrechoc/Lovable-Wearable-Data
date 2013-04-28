<?php

$q= $_GET["q"];

// echo $q;

$pieces = explode(" ", $q);

include('../connectionheader.php');

$first_name = $pieces[0]; 
$last_name = $pieces[1]; 
$adoption_date = Date("Y")."-".Date("m")."-".Date("d");
$ID = $pieces[2]; 

if ( 1==1 ){

$already = 0;
//check if the same name already exists
$q="SELECT * FROM exolist WHERE first_name = '".$first_name."' AND last_name = '".$last_name."'";
$page_imp_r=mysql_query($q) or print(mysql_error());
while ($page_imp=mysql_fetch_array($page_imp_r)){
  	$already = 1;
   //echo  "found: ".$page_imp['ID']." > name: ". $page_imp['name'].". mass: ". round($page_imp['mass'],2).". radius: ". round($page_imp['radius'],2)." temp: ". round($page_imp['temperature']).". period:". round($page_imp['period'],2);
 }

 //check if the planet is already adopted
$q="SELECT * FROM exolist WHERE ID = '".$ID."'  ";
$page_imp_r=mysql_query($q) or print(mysql_error());
while ($page_imp=mysql_fetch_array($page_imp_r)){
  if ( $page_imp['first_name'] != "" )
   $already = 2;
   //echo  "found: ".$page_imp['ID']." > name: ". $page_imp['name'].". mass: ". round($page_imp['mass'],2).". radius: ". round($page_imp['radius'],2)." temp: ". round($page_imp['temperature']).". period:". round($page_imp['period'],2);
 }
 

 //not adopted not in the namelist
if ( $already == 0)
{
 
		$q="UPDATE exolist 
		SET first_name='".$first_name."', last_name='".$last_name."', adoption_date='".$adoption_date."'
		WHERE ID='".$ID."'  ";
		$page_imp_r=mysql_query($q) or print(mysql_error());
		echo $first_name ." ".$last_name ." done, thx!";
}
  
//more than one planet
  if ( $already == 1)
		echo "sorry, one planet only!";

//already adopted
 if ( $already == 2)
		echo "already adopted!";
 
 }else
 { echo "please gie a name"; }
 

mysql_close();
 
?> 

  