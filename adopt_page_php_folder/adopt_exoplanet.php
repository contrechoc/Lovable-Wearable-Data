
<!DOCTYPE html>
<html>
	<head>
		<link type="text/css" rel="stylesheet" href="stylesheet.css"/>
		<title>Adopt a lonely exoplanet!</title>
	</head>
	<body>

<form id="frmCaptcha" name="frmCaptcha">
<table>
  	<TR>
    	<TD align="left">
    	 
      <label for="captcha"> <CENTER>
      <h2>Adopt a lonely exoplanet!</h2>
      <img class="adopt" src="http://www.contrechoc.com/exovest/adopt.png"  > </label>
     
      This is an ART project! (Not a commercial scheme )
        </TD>
    	<TD align="left">
    	All these planets that are discovered never have been looked at by a human being.
    	To generate interest for these other worlds, we have made the data visible on a wearable (see below)
    	And you can also team up with one of these planets to have a personal relationship!<BR><BR>
    	<a href="#ref1">See reference 1!</a>
    </TD>
	</TR>
	<TR>
  		<TD>
    		<LABEL for="captcha">
    		1. Insert your name (or pseudo) in the list of adopters of exoplanets.<BR>
    		2. generate a subset of the exoplanets you like, big, small, hot, cold...<BR>
    		3. choose one of the planets from this list you fancy<BR>
    		5. reproduce the captcha to show you are not an alien<BR>
    		6. hit submit, reload the page and your name is in the list<BR>
    		7. your name will appear under the data of the planets on the exovest<BR><BR>
    		   <img class="dots2" src="http://www.contrechoc.com/exovest/dots.png"  > 
   			<a href="#ref2"> See reference 2!
   			</a>
    		</LABEL>
		</TD>
    	<TD>
       		<TABLE>
       			<TR>
   					<TD>
    				first name:
 					</TD>
 					<TD>
 					<input id="name1" type="text" name="name1" value="<?php 
 	  					$nameVal =  $_REQUEST["name1"] ;
    					echo  $nameVal;
    				?>"	maxlength="15" size="32" /> 
    				</TD>
      			</TR>
  				<TR>
     				<TD>
    					last name:  
    				</TD>
  					<TD>
					<input id="name2" type="text" name="name2" value=" <?php 
 	  				$nameVal =   $_REQUEST["name2"] ;
    				echo  $nameVal;
    				?>"maxlength="15" size="32" />
    				</TD>
      			</TR>
      		</TABLE>
    	</TD>
      </TR> 
<TR>
  
    <TD colspan="2" >

   The current subset of exoplanets :  
  <B>
    <?php 
    echo $indication=htmlentities($_REQUEST["indication"], ENT_NOQUOTES);
    ?>
  </B><BR>
 		Select one of this list, or select another subset
  <TABLE>
  	<TR>
   		<TD>
   	
			 <?php
echo htmlentities($_REQUEST["indication"], ENT_NOQUOTES);
?>
 
 			exoplanet:</B> <BR>
 
			<SELECT ID="exoPlanet" SIZE="10" MULTIPLE >
			<OPTION SELECTED>
   			 <?php 
 
$indication = "habitable";

include('../makeYourOwnConnectionToYourDataBase.php');
 
$indication=htmlentities($_REQUEST["indication"], ENT_NOQUOTES);

$q ="";
if ( $indication == "habitable"){
$q ="SELECT * FROM `exolist` 
WHERE mass < 1.75 AND period > 10 AND period < 500 AND radius > .5 AND radius < 3 AND temperature < 500
ORDER BY mass ASC
";
}
 
if ( $indication == "inhabitable"){
$q ="SELECT * FROM `exolist` 
WHERE mass > 2 AND  radius > 1 AND temperature > 500
ORDER BY temperature DESC
";
}

if ( $indication == "big"){
$q ="SELECT * FROM `exolist` 
WHERE mass > 10
ORDER BY mass DESC
";
}

if ( $indication == "small"){
$q ="SELECT * FROM `exolist` 
WHERE mass < 0.1
ORDER BY mass ASC
";
}

if ( $indication == "hot"){
$q ="SELECT * FROM `exolist` 
WHERE temperature > 1000
ORDER BY temperature DESC
";
}
 
if ( $indication == "cold"){
$q ="SELECT * FROM `exolist` 
WHERE temperature < 500
ORDER BY temperature ASC
";
}

 $page_imp_r=mysql_query($q) or print(mysql_error());
  while ($page_imp=mysql_fetch_array($page_imp_r)){
  if ( round($page_imp['mass'],2) > 0.0 )
  echo  "<OPTION>".$page_imp['id']." > name: ". $page_imp['name'].". mass: ". round($page_imp['mass'],2).". radius: ". round($page_imp['radius'],2)." temp: ". round($page_imp['temperature']).". period:". round($page_imp['period'],2);
  else
   echo  "<OPTION>".$page_imp['id']." > name: ". $page_imp['name'].". mass: " ." - ".". radius: ". round($page_imp['radius'],2)." temp: ". round($page_imp['temperature']).". period:". round($page_imp['period'],2);
 
  }  

mysql_close();
?>
			</SELECT>
 
  		</TD>
     	<TD>
		Select a subset you fancy:
		<TABLE>
    		<TR>
    			<TD>
					<input id="radio" type="radio" name="indication" value="habitable">habitable<BR>
					<input id="radio" type="radio" name="indication" value="inhabitable">inhabitable
    				<input id="radio" type="radio" name="indication" value="big">big<BR>
					<input id="radio" type="radio" name="indication" value="small">small
    			</TD>
     			<TD>
    				<input id="radio" type="radio" name="indication" value="hot">hot<BR>
					<input id="radio" type="radio" name="indication" value="cold">cold
    			</TD>
     			<TD>
      				<input id="btnProp" type="button" value="Submit properties" name="btnProp"
          			onclick="getList(document.frmCaptcha)" />
   				</TD>
     		</TR>
		</TABLE>
    	<a href="#ref3">
     
    See reference 3!</a>
    </TD>
    </TR>
		</TABLE>
		
    </TD>
    </TR>
    
  <TR>
    <TD>
      <img id="imgCaptcha" src="create_image.php" />
    </TD>
     <TD>
       To check your alien factor, give us the captcha
    	<input id="txtCaptcha" type="text" name="txtCaptcha" value="" maxlength="10" size="32" />
    </TD>
  </TR>
  
  <TR>
    <TD ID="choice" > 
      <input id="btnCaptcha" type="button" value="Submit Name and Choice" name="btnCaptcha"
          onclick="getParam(document.frmCaptcha)" />
    </TD>
 
    <TD>
    The result of the complicated adoption process will be shown here:
   
    <div id="result">Result field...</div>
    </TD>
  </TR>
  
  
   <TR>
   <TD><TABLE><TR>
    <td columns='2'>References:</TD>
     </TR>
  
  
   <TR>
    <TD><a name="ref1">
   1.  <BR>     This page is part of the exovest project:<BR>
      <a href="http://myfablab.wordpress.com/2013/04/23/nasa-challenge-final-result/">http://myfablab.wordpress.com/2013/04/23/nasa-challenge-final-result/</a><BR>
      This is an artists project meant to be "fun", that is raising awareness of SPACE and seeking to connect space event with the "human" experience.
 
    </TD> </TR>
  
  
   <TR>
        <TD><a name="ref2">
  2. <BR>  Your name will be accepted only once. <BR>
    No email required (these is an ART project - not something else) <BR>
    No donations or money involved.<BR>
    No rights can be bla bla bla.... <BR>
    This project can be aborted at any time, in any stage.
    Your name will be associated with the planet and could be displayed on the exovest.<BR>
    No proof of this is available :-)<BR>
    (But it works....feel free to copy, modify, sell, fall asleep or do nothing)
  
    </TD> </TR>
  
  
   <TR>
     <TD><a name="ref3">
  3.  <BR>  Exoplanets from this source:<BR>
   <a href = "https://raw.github.com/hannorein/oec_tables/master/comma_separated/open_exoplanet_catalogue.txt">https://raw.github.com/hannorein/oec_tables/master/comma_separated/open_exoplanet_catalogue.txt</a>
   <BR>
   To link it witch names, this list is in a separate database.<BR>
   (Do not mess around with Holy Lists!)<BR>
  
    </TD> </TR>
  
  
   <TR>
     <TD><a name="ref4">
  4.
   <img class="dots" src="http://www.contrechoc.com/exovest/dots.png"  > 
    </TD>
    
  </TR>
  </TABLE>
   </TD>
    <TD>
     These exoplanets are now having a firend on earth and don't feel so lonely anymore :-)<BR><BR>
   List of adoptations:<BR><BR>
   <TABLE><TR><TD>
  
    <?php 
 
include('../connectionheader.php');
 
$q ="SELECT * FROM `exolist` 
WHERE first_name != ''";

 $page_imp_r=mysql_query($q) or print(mysql_error());
  while ($page_imp=mysql_fetch_array($page_imp_r)){

   echo   $page_imp['first_name']." ". $page_imp['last_name']." adopted ".$page_imp['name']."<BR>";
 
  }  

mysql_close();
?>
 </TD>
    
  </TR>
  
  </TABLE>
    </TD>
  <TR>
   <TD>
   <img class="vest" src="http://www.contrechoc.com/exovest/exovestO.jpg"  > 
    </TD>
     <TD ID="advanced" class="circle">
     Made by Beam Contrechoc <BR>
     <a href="http://www.contrechoc.com">www.contrechoc.com</a><BR>
     Showed (and developed) during the NASA Hackathon<BR>
      <img class="logo" src="http://www.contrechoc.com/exovest/logo.png"  > 
     2013<BR>
      </TD>
   </TR>
</table> 
</form>

<script language="JavaScript" type="text/javascript" src="ajax_captcha.js"></script>
	</body>
</html>