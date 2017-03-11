<?php

//*************************************************************************************************
// SOUS-CONTROLEUR DE LA CONSULTATION                                                             *
// + fonction d'affichage du formulaire de consultation                                           *
// + fonction d'affichage de la liste des appels                                                  *
//*************************************************************************************************


//============================================================
// FONCTIONS
//============================================================


//------------------------------------------------------------------
// Fonction d'affichage de formulaire de consultation

function consultation_formulaire ($date1, $date2)
{
  global $conn;


  $html = <<<END

  <SCRIPT LANGUAGE="JAVASCRIPT">

      function verifie_dates()
      {
        // variables
        var chpdate1 = window.document.forms["consult"].elements["date1"].value ;
        var chpdate2 = window.document.forms["consult"].elements["date2"].value ;
        var date1 = new Date(chpdate1.substring(6,10), chpdate1.substring(3,5)-1, chpdate1.substring(0,2)) ; //formatage pour JS
        var date2 = new Date(chpdate2.substring(6,10), chpdate2.substring(3,5)-1, chpdate2.substring(0,2)) ; //formatage pour JS

        // test du champ date1
        if ( chpdate1.length == 0 )
        {
          alert("Vous avez oublié de remplir le premier champ !" ) ;
          return false;
        }
        if ( chpdate1.length != 10
              || (chpdate1.charAt(2) != "/") || (chpdate1.charAt(5) != "/")
              || isNaN(chpdate1.substring(0,2))
              || isNaN(chpdate1.substring(3,5))
              || isNaN(chpdate1.substring(7,10))
            )
        {
          alert("Attention, le premier champ est mal rempli : JJ/MM/AAAA");
          return false;
        }

        // test du champ date2
        if ( chpdate2.length !=0
            &&  (chpdate2.length != 10
                  || (chpdate2.charAt(2) != "/") || (chpdate2.charAt(5) != "/")
                  || isNaN(chpdate2.substring(0,2))
                  || isNaN(chpdate2.substring(3,5))
                  || isNaN(chpdate2.substring(7,10))
                )
            )
        {
          alert("Attention, le deuxième champ est mal rempli : JJ/MM/AAAA") ;
          return false;
        }
        // on vérifie que la première date est plus vieille que la première
        if (date1>date2)
        {
          alert ("Votre deuxième date doit être postérieure à la première !") ;
          return false;
        }
        else
        {
          return true ;
        }
      }
  </SCRIPT>


  <!------------------------------------------------>
  <!-- JAVASCRIPTS nécessaires pour le calendrier -->
  <!------------------------------------------------>
  <script language="javascript" src="javascript/cal2.js"></script>
  <script language="javascript" src="javascript/cal_conf2.js"></script>


  <H1>Je veux consulter les appels reçus entre :</H1>

  <FORM METHOD="GET" name="consult" onSubmit="return verifie_dates()">
    <INPUT TYPE="hidden" NAME="action" VALUE="consulter.liste">
    <TABLE class="REQUETE" align="center">
      <TR>
        <TD>le</TD>
        <TD><INPUT TYPE="text" SIZE="10" NAME="date1" VALUE="$date1">&nbsp;&nbsp;JJ/MM/AAAA&nbsp;&nbsp;&nbsp;<IMG src="images/calendrier.jpg" align="middle" onMouseUp="javascript:showCal('Calendrier2')"></TD>
      </TR>
      <TR>
        <TD>et le :</TD>
        <TD><INPUT TYPE="text" SIZE="10" NAME="date2" VALUE="$date2">&nbsp;&nbsp;JJ/MM/AAAA&nbsp;&nbsp;&nbsp;<IMG src="images/calendrier.jpg" align="middle" onMouseUp="javascript:showCal('Calendrier3')"></TD>
      </TR>
    </TABLE>
    <BR>
    <CENTER><INPUT TYPE="submit" VALUE="Envoyer"></CENTER>
  </FORM>

END;

  return(array($html, null));
}



//------------------------------------------------------------------
// Fonction d'affichage de la liste des appels

function consultation_liste ($date1, $date2, $provenance="")
{
  global $conn ;

  if ($date2=="")
  {
    // on récupère la date courante
    $ladate2=getdate() ;
    // pour que le jour soit toujours au format JJ
    if ($ladate2['mday']<10)
    {
      $jour = "0".$ladate2['mday'] ;
    }
    else
    {
      $jour = $ladate2['mday'] ;
    }
    // pour que le mois soit toujours au format MM
    if ($ladate2['mon']<10)
    {
      $mois = "0".$ladate2['mon'] ;
    }
    else
    {
      $mois = $ladate2['mon'] ;
    }
    $annee = $ladate2['year'] ;
    $date2 = "$jour/$mois/$annee" ;
  }

  // gestion des dates au format de la base
  $date1_split = explode('/',$date1) ;
  $date2_split = explode('/',$date2) ;
  $date1formatee = mktime (0, 0, 0, $date1_split[1], $date1_split[0], $date1_split[2]) ;
  $date2formatee = mktime (23, 59, 59, $date2_split[1], $date2_split[0], $date2_split[2]) ;


  $sql =  "SELECT a.id, a.date_appel, s.nom, l.nom, p.nom ".
            "FROM appel a, structure s, lieu l, personne p ".
            "WHERE a.id_structure = s.id ".
            "AND a.id_lieu = l.id ".
            "AND a.id_personne = p.id ".
            "AND a.date_appel < ".$conn->DBTimeStamp($date2formatee).
            "AND a.date_appel > ".$conn->DBTimeStamp($date1formatee).
            "ORDER BY a.date_appel DESC";


  // on limite le nombre de résultats à 999
  $gSQLBlockRows = 999;

  $rs = $conn->Execute($sql); // Execute the query and get the empty recordset

  if (!$rs)
  {
    print $conn->ErrorMsg() ;
  }

  $nb = $rs->RowCount("appel") ;
  // si on n'a pas de résultat à notre requête
  if (($nb) == NULL)
  {
    // selon dans quelle page on se situe (modification ou saisie) on affiche un message différent
    if ($_REQUEST["action"]!="consulter")
    {
      echo "<H2>Il n'y a pas de r&eacute;sultat correspondant &agrave; votre recherche</H2>" ;
      echo "<H3><A HREF=\"index.php?action=consulter&date1=$date1&date2=$date2\">Retour</A><H3>" ;
    }
    else
    {
      echo "<H2>Aucun appel n'a encore &eacute;t&eacute; saisi.</H2>" ;
    }
    echo "<BR><BR>" ;
  }
  // sinon, génération du tableau
  else
  {
    echo ("<TABLE class=\"RESULTAT\" align=\"center\">
           <TR><TH>Date</TH><TH>Heure</TH><TH>Structure appelante</TH><TH>Localisation</TH><TH>Appelant</TH></TR>   ") ;
    while (!$rs->EOF)
    {
      $timestamp = $conn->UnixTimeStamp($rs->fields[1]) ;
      $date = date (("d/m/Y"),$timestamp) ;
      $heure = date (("H:i"),$timestamp) ;
      echo "<TR>" ;
      echo "  <TD>&nbsp;".$date."</TD><TD>&nbsp;".$heure."</TD>" ;
      echo "  <TD>&nbsp;".$rs->fields[2]."</TD><TD>&nbsp;".$rs->fields[3]."</TD><TD>&nbsp;".$rs->fields[4]."</TD>" ;
      echo "  <TD><A HREF=\"index.php?action=consulter.fiche&idAppel=".$rs->fields[0]."&date1=".$date1."&date2=".$date2."&provenance=".$provenance."\">Plus d'infos</A></TD>" ;
      echo "  <TD><A HREF=\"index.php?action=saisir.modifier&idAppel=".$rs->fields[0]."&date1=".$date1."&date2=".$date2."&provenance=".$provenance."\" >Modifier</A></TD>" ;
      $rs->MoveNext() ;
      echo "</TR>" ;
    }
    echo "</TABLE>" ;
    echo "<BR>" ;
    // pour avoir le lien de retour uniquement si on est sur la page de listage des appels,
    // et non sur la première page de consultation avec le formulaire
    if ($_REQUEST["action"]!="consulter")
    {
      echo "<CENTER>" ;
      echo "  <A HREF=\"index.php?action=consulter&date1=".$date1."&date2=".$date2."\">Retour</A>" ;
      echo "</CENTER>" ;
    }
    echo "<BR>" ;
  }
}



//=======================================================
// PROGRAMME PRINCIPAL
//=======================================================

function consulter_switch($action)
{
  global $conn ;
  global $_REQUEST ;
  global $action ;
  global $redirect ;
  global $footer_msg ;


  switch ($action)
  {
    //-------------------------------------------------------------------------------------------
    case "consulter":

      $res = consultation_formulaire ($_REQUEST["date1"], $_REQUEST["date2"]) ;
      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        // on affiche le formulaire de consultation
        print $res[0] ;
        // on affiche les appels de la journée
        echo "<BR><H2>Liste des appels de la journ&eacute;e</H2>" ;
        // on récupère la date d'aujourd'hui
        $ladate = getdate() ;
        // pour que le jour soit toujours au format JJ
        if ($ladate['mday']<10) { $jour = "0".$ladate['mday'] ; }
        else { $jour = $ladate['mday'] ; }
        // pour que le mois soit toujours au format MM
        if ($ladate['mon']<10) { $mois = "0".$ladate['mon'] ; }
        else { $mois = $ladate['mon'] ; }
        // l'année est toujours au format AAAA
        $annee = $ladate['year'] ;
        // on formate la date
        $date1 = "$jour/$mois/$annee" ;
        $date2 = "$jour/$mois/$annee" ;
        // on indique où l'on est
        //(utile pour que les liens de retour
        // affectent à des endroits différents selon d'où l'on vient)
        $provenance="consultation" ;
        // liste des appels de la date d'aujourd'hui
        consultation_liste ($date1, $date2,$provenance) ;
      }
      break;
    //-------------------------------------------------------------------------------------------
    case "consulter.liste":
      // on affiche la liste d'appels
      $rs = consultation_liste($_REQUEST["date1"], $_REQUEST["date2"]) ;
      if($rs[1] != null)
      {
        print error($rs[1]) ;
      }
      else
      {
        print ($rs[0]) ;
      }
      break;
    //-------------------------------------------------------------------------------------------
    case "consulter.fiche":
      // on affiche la fiche d'appel
      $res = afficher_appel() ;
      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        print ($res[0]) ;
      }
      break;
    //-------------------------------------------------------------------------------------------
    default:
    $action="consulter" ;
  }
}

?>
