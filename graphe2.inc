<?php

//============================================================
// FONCTION
//============================================================


//------------------------------------------------------------------
// Fonction d'affichage des statistiques numéro 2

function states_choix2($date1,$date2)
{
  global $conn ;

  // on récupère les deux dates entrées
  $date1 = $_REQUEST["date1"] ;
  $date2 = $_REQUEST["date2"] ;

  // on met en session les deux dates entrées
  $_SESSION["date1"] = $date1 ;
  $_SESSION["date2"] = $date2 ;

  // gestion des dates au format de la base
  $date1_split = explode('/',$date1) ;
  $date2_split = explode('/',$date2) ;

  $date1_formatee = mktime (0, 0, 0, $date1_split[1], $date1_split[0], $date1_split[2]) ;
  $date2_formatee = mktime (23, 59, 59, $date2_split[1], $date2_split[0], $date2_split[2]) ;


  // première requête pour aller chercher le nombre total d'appels entre les deux dates

  $req =  "SELECT count(id) FROM appel ".
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
    // deuxième requête qui va générer un tableau avec les nombres correspondants à chaque tranche horaire (uniquement si elles existent)

    //                (heure)       |       (heure+1)     | (nombre)
    $sql =  "SELECT hour(date_appel), (hour(date_appel))+1, count(*) ".
            "FROM appel ".
            "WHERE date_appel >".$conn->DBTimeStamp($date1_formatee).
            "AND date_appel <".$conn->DBTimeStamp($date2_formatee).
            "GROUP BY hour(date_appel)";

    $res = $conn->Execute($sql);
    
    // affichage des résultats
    echo "<H2>Répartition des appels par tranche horaire passés entre le ".$date1." et le ".$date2."</H2><BR>" ;
    // début du tableau
    echo "<TABLE class=\"RESULTAT\" align=\"center\"><TR><TH>Tranche horaire</TH><TH>Nombre</TH></TR>" ;
    // initialisation de l'indice à 1
    $i=1 ;
    while (!$res->EOF)
    {
      // on construit chaque ligne du tableau
      //                  (différentes tranches horaires)
      //                                                     (différentes tranches horaires+1heure)
      //                                                                                                   (nombre d'appels)
      echo  "<TR><TD class=\"RIGHT\">".$res->fields[0]."h - ".($res->fields[1])."h</TD><TD class=\"RIGHT\">".$res->fields[2]."</TD></TR>" ;
      // pour permettre le passage de paramètres via l'url pour la génération de graphiques
      $graphea = $graphea."&title_".$i."=".urlencode($res->fields[0])."h-".($res->fields[1])."h"."&val_".$i."=".urlencode($res->fields[2]) ;
      // on se déplace vers la prochaine valeur
      $res->MoveNext();
      $i=$i+1;
    }
      // on met en session les différents paramètres nécessaires à la construction des graphiques
      $_SESSION["graphea"]= $graphea ;
      $_SESSION["grapheb"]= $grapheb ;
    // fin du tableau
    echo "</TABLE>" ;

    echo "<BR><BR>" ;
    echo "<CENTER>" ;

    // on appelle le fichier fig2a qui va générer le graphique 2a
    // on lui passe les paramètres "title" et "val"
    echo "<IMG src=\"fig2a.php?_jpg_csimd=1".$graphea."\" border=0>" ;

    echo "<BR><BR>" ;

    // liens pour ajouter au panier, visualiser le panier, et retourner au formulaire de statistiques
    echo "<A HREF=\"index.php?action=panier.ajouter&type=graphe2&titre=".urlencode("Répartition des appels par tranche horaire")."\">Ajouter au panier&nbsp;|&nbsp;</A>" ;
    echo "<A HREF=\"index.php?action=panier.lister\">Visualiser le panier&nbsp;|&nbsp;</A>" ;
    echo "<A HREF=\"index.php?action=states&date1=$date1&date2=$date2\">Retour</A>" ;

    echo "</CENTER>" ;

    echo "<BR><BR>" ;
    echo "<BR><BR>" ;
  }
}

?>