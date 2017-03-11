<?php
session_start();
//============================================================
// FONCTION
//============================================================

//Tableaux qui vont donner le jour et le mois textuellement

$mois = array("Janvier", "Février", "Mars", "Avril", "Mai", "Juin", "Juillet", "Août", "Septembre", "Octobre", "Novembre","Décembre");
$jour= array("Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi");

//------------------------------------------------------------------
// Fonction d'affichage des statistiques numéro 4

function states_choix4($date1,$date2)
{
  global $conn;
  global $jour;
  global $mois;

  // on récupère les deux dates entrées
  $date1 = $_REQUEST["date1"];
  $date2 = $_REQUEST["date2"];

  // on met en session les deux dates entrées
  $_SESSION["date1"] = $date1;
  $_SESSION["date2"] = $date2;

  // gestion des dates au format de la base
  $date1_split = explode('/',$date1) ;
  $date2_split = explode('/',$date2) ;

  $date1_formatee = mktime (0, 0, 0, $date1_split[1], $date1_split[0], $date1_split[2]) ;
  $date2_formatee = mktime (23, 59, 29, $date2_split[1], $date2_split[0], $date2_split[2]) ;



//Boucle permettant de récupérer les types

  $title = array();
     
  $req =  "SELECT id, libelle FROM type";
  $res = $conn->Execute($req);
  while (!$res->EOF)                                

  {
    $title[$res->fields[0]] = $res->fields[1];
    $res->MoveNext();
  }
  	// On sérialize et on met en session le tab title
	$stitle = serialize($title); 
	$_SESSION['title']=$stitle;


 //Boucle permettant de récupérer les dates tapées par l'utilisateur

  $abs = array();
    
  for($date_current = $date1_formatee; $date_current <= $date2_formatee; $date_current += 24*60*60) {
    $ldate = localtime($date_current, true);
    array_push($abs, $jour[$ldate["tm_wday"]]." ".$ldate["tm_mday"]." ".$mois[$ldate["tm_mon"]]);
  }

  //print_r($abs);

  	// On sérialize et on met en session le tab abs
  	$sabs = serialize($abs); 
  	$_SESSION ["abs"] = $sabs;


 //Boucle permettant de récupérer les données correspondantes

  $data = array();

  foreach($title as $key1 => $title_data) {
  	$data[$key1] = array();
  	foreach($abs as $key2 => $abs_data) {
  		$data[$key1][$key2] = 0;
  	}
  }
  

  // première requête pour aller chercher le nombre total d'appels entre les deux dates

  $req =  "SELECT count(appel.id) FROM appel ".
          "WHERE (appel.date_appel>".$conn->DBTimeStamp($date1_formatee).
              "AND appel.date_appel< ".$conn->DBTimeStamp($date2_formatee).") " ;

  $rs = $conn->Execute($req);
  $total = $rs->fields[0] ;
  
  
  // si on n'a pas de résultat à afficher
  if ($total==0)
  {
    echo "<H2>Il n'y a pas de résultat correspondant à votre recherche</H2>" ;
    echo "<H3><A HREF=\"index.php?action=states&date1=$date1&date2=$date2\">Retour</A><H3>" ;
    echo "<BR><BR>" ;
  }
  else
  {

    // deuxième requête qui va générer un tableau avec les nombres correspondant à chaque type d'appels et par jour (uniquement si ils existent)
                  
   $sql="SELECT count(appel.id), type.id, date_format(date_appel, '%e/%m/%y') ".
	"FROM appel, type ".
	"WHERE (appel.id_type = type.id) ".
	"AND (substring(date_appel, 1, 10) > ".$conn->DBTimeStamp($date1_formatee).") ".
	"AND (substring(date_appel, 1, 10) < ".$conn->DBTimeStamp($date2_formatee).") ".
	"group by substring(date_appel,1,10), type.id";

   $res = $conn->Execute($sql);

   while(!$res->EOF) {
 	$date_split = explode('/',$res->fields[2]);
 	$date_current = mktime (0, 0, 0, $date_split[1], $date_split[0], $date_split[2]);
 	$indice = ($date_current - $date1_formatee)/(24*60*60); 	
 	$data[$res->fields[1]][$indice] = $res->fields[0];
 	$res->MoveNext();
  }
  
  	$sdata = serialize($data); 
 	$_SESSION ["data"] = $sdata;
  
    // affichage des résultats
    echo "<H2>Répartition des appels par jour et par type entre le ".$date1." et le ".$date2."</H2>" ;
    echo "<BR><BR>" ;

   // on met en session les différents paramètres nécessaires à la construction des graphiques
    $graphea = array('title' => $title, 'abs' => $abs, 'data' => $data);
    $sgraphpanier = serialize($graphpanier); 
    $_SESSION ["graphea"] = $sgraphpanier;


   echo "<IMG src=\"fig4.php?_jpg_csimd=1\" border=0><BR><BR>" ;
   echo "<BR><BR>" ;
   echo "<CENTER>" ;

    // liens pour ajouter au panier, visualiser le panier, et retourner au formulaire de statistiques
    echo "<A HREF=\"index.php?action=panier.ajouter&type=graphe4&titre=".urlencode("Répartition des appels par jour et par type")."\">Ajouter au panier&nbsp;|&nbsp;</A>" ;
    echo "<A HREF=\"index.php?action=panier.lister\">Visualiser le panier&nbsp;|&nbsp;</A>" ;
    echo "<A HREF=\"index.php?action=states&date1=$date1&date2=$date2\">Retour</A>" ;

    echo "</CENTER>" ;
        
    echo "<BR><BR>" ;
    echo "<BR><BR>" ;
  }
}

?>