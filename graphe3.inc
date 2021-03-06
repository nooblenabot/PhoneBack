<?php

//============================================================
// FONCTION
//============================================================


//------------------------------------------------------------------
// Fonction d'affichage des statistiques num�ro 3

function states_choix3($date1,$date2)
{
  global $conn;

  // on r�cup�re les deux dates entr�es
  $date1 = $_REQUEST["date1"];
  $date2 = $_REQUEST["date2"];

  // on met en session les deux dates entr�es
  $_SESSION["date1"] = $date1;
  $_SESSION["date2"] = $date2;

  // gestion des dates au format de la base
  $date1_split = explode('/',$date1) ;
  $date2_split = explode('/',$date2) ;

  $date1_formatee = mktime (0, 0, 0, $date1_split[1], $date1_split[0], $date1_split[2]) ;
  $date2_formatee = mktime (23, 59, 29, $date2_split[1], $date2_split[0], $date2_split[2]) ;


  // premi�re requ�te pour aller chercher le nombre total d'appels entre les deux dates

  $req =  "SELECT count(id) FROM appel ".
          "WHERE (appel.date_appel>".$conn->DBTimeStamp($date1_formatee).
              "AND appel.date_appel< ".$conn->DBTimeStamp($date2_formatee).") " ;

  $rs = $conn->Execute($req);
  $total = $rs->fields[0] ;

  // si on n'a pas de r�sultat � afficher
  if ($total==0)
  {
    echo "<H2>Il n'y a pas de r�sultat correspondant � votre recherche</H2>" ;
    echo "<H3><A HREF=\"index.php?action=states&date1=$date1&date2=$date2\">Retour</A><H3>" ;
    echo "<BR><BR>" ;
  }
  else
  {
    // deuxi�me requ�te qui va g�n�rer un tableau avec les nombres correspondant � chaque appelant (uniquement si ils existent)

    //                (nom)      |   (nombre)     |           (pourcentage)
    $sql =  "SELECT structure.nom, count(appel.id), (count(appel.id)/".$total."*100) ".
            "FROM  structure, appel ".
            "WHERE structure.id = appel.id_structure ".
                "AND(appel.date_appel>".$conn->DBTimeStamp($date1_formatee).
                "AND appel.date_appel< ".$conn->DBTimeStamp($date2_formatee).") ".
            "GROUP BY structure.nom ".
            "ORDER BY structure.nom DESC " ;

    $res = $conn->Execute($sql);

    // affichage des r�sultats
    echo "<H2>R�partition des structures appelantes entre le ".$date1." et le ".$date2."</H2>" ;
    // d�but du tableau
    echo "<TABLE class=\"RESULTAT\" align=\"center\"><TR><TH>Type</TH><TH>Nombre</TH><TH>Pourcentage</TH></TR>" ;
    // initialisation de l'indice � 1
    $i=1 ;
    while (!$res->EOF)
    {
      // on construit chaque ligne du tableau
      //                    (nom)           |                            (nombre)        |                       (pourcentage)
      echo  "<TR><TD>".$res->fields[0]."</TD><TD class=\"RIGHT\">".$res->fields[1]."</TD><TD class=\"RIGHT\">".$res->fields[2]."</TD></TR>" ;
      $pourcentotal = $pourcentotal+($res->fields[2]);

      // pour permettre le passage de param�tres via l'url pour la g�n�ration de graphiques
      $graphea = $graphea."&title_".$i."=".urlencode($res->fields[0])."&val_".$i."=".urlencode($res->fields[2]) ;
      $grapheb = $grapheb."&title_".$i."=".urlencode($res->fields[0])."&val_".$i."=".urlencode($res->fields[1]) ;
      // on se d�place vers la prochaine valeur
      $res->MoveNext();
      $i=$i+1;
    }
    
    // Ces graphes sont en fait mis en session pour pouvoir �tre r�cup�r� quand on g�n�re le PDF	
    // on met en session les diff�rents param�tres n�cessaires � la construction des graphiques
   $_SESSION["graphea"]= $graphea ;
   $_SESSION["grapheb"]= $grapheb ;

    // derni�re ligne du tableau : affichage des totaux
    echo    "<TR><TD class=\"TOTAL\">Total</TD><TD class=\"TOTAL\">".$total."</TD><TD class=\"TOTAL\">".$pourcentotal."</TD></TR>" ;
    echo "</TABLE>" ;
    
    echo "<BR><BR>" ;
    echo "<CENTER>" ;

    // on appelle le fichier fig3a qui va g�n�rer le graphique 3a
    // on lui passe les param�tres "title" et "val"
    echo "<IMG src=\"fig3a.php?_jpg_csimd=1".$graphea."\" border=0>" ;
    echo "<BR><BR>" ;
    echo "<BR><BR>" ;
    // on appelle le fichier fig3b qui va g�n�rer le graphique 3b
    // on lui passe les param�tres "title" et "val"
    echo "<IMG src=\"fig3b.php?_jpg_csimd=1".$grapheb."\" border=0>" ;
    echo "<BR><BR>" ;

    // liens pour ajouter au panier, visualiser le panier, et retourner au formulaire de statistiques
    echo "<A HREF=\"index.php?action=panier.ajouter&type=graphe3&titre=".urlencode("R�partition des structures appelantes")."\">Ajouter au panier&nbsp;|&nbsp;</A>" ;
    echo "<A HREF=\"index.php?action=panier.lister\">Visualiser le panier&nbsp;|&nbsp;</A>" ;
    echo "<A HREF=\"index.php?action=states&date1=$date1&date2=$date2\">Retour</A>" ;

    echo "</CENTER>" ;
    
    echo "<BR><BR>" ;
    echo "<BR><BR>" ;
  }
}

?>