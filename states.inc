<?php

//*************************************************************************************************
// SOUS-CONTROLEUR DES STATISTIQUES                                                               *
// + fonction d'affichage du formulaire de statistiques                                           *
//*************************************************************************************************

include ("graphe1.inc") ;
include ("graphe2.inc") ;
include ("graphe3.inc") ;
include ("graphe4.inc") ;

//============================================================
// FONCTION
//============================================================


//------------------------------------------------------------------
// Fonction d'affichage du formulaire de states

function states_formulaire($choix, $date1, $date2)
{
  $html = <<<END

  <SCRIPT LANGUAGE="JAVASCRIPT">

      function verifie_dates()
      {
        // variables
        var chpdate1 = window.document.forms["states_formulaire"].elements["date1"].value ;
        var chpdate2 = window.document.forms["states_formulaire"].elements["date2"].value ;
        var date1 = new Date(chpdate1.substring(6,10), chpdate1.substring(3,5)-1, chpdate1.substring(0,2)) ; //formatage pour JS
        var date2 = new Date(chpdate2.substring(6,10), chpdate2.substring(3,5)-1, chpdate2.substring(0,2)) ; //formatage pour JS

        // test de la conformite du champ date1 rempli
        if  (chpdate1.search(/^[0-9]{2}\/[0-9]{2}\/[0-9]{4}$/) == -1)
        {
          alert("Veuillez saisir un format de date valide : JJ/MM/AAAA pour le premier champ de date");
          return false ;
        }
        // test de la conformite du champ date2 rempli
        if  (chpdate2.search(/^[0-9]{2}\/[0-9]{2}\/[0-9]{4}$/) == -1)
        {
          alert("Veuillez saisir un format de date valide : JJ/MM/AAAA pour le deuxième champ de date");
          return false ;
        }
        // on vérifie que la première date est plus vieille que la première
        if (date1>date2)
        {
          alert ("Votre deuxième date doit être postérieure à la première !");
          return false;
        }
        else
        {
          return true;
        }
      }

  </SCRIPT>
  
  
  <!-- JAVASCRIPTS nécessaires pour le calendrier -->
  <script language="javascript" src="javascript/cal2.js"></script>
  <script language="javascript" src="javascript/cal_conf2.js"></script>
  
  
  <H1>Je désire consulter les statistiques qui concernent :</H1>

  <FORM METHOD="GET" name="states_formulaire" onSubmit="return verifie_dates()">
  <TABLE align="center">
    <TR>
      <TD><INPUT TYPE="hidden" NAME="action" VALUE="states.choix"></TD>
    <TR>
    <TR>
      <TD><INPUT TYPE="radio" NAME="choix" VALUE="1" checked></TD>
      <TD>la répartition des appels selon le type</TD>
    </TR>
    <TR>
      <TD><INPUT TYPE="radio" NAME="choix" VALUE="2"></TD>
      <TD>la répartition moyenne des appels par tranche horaire</TD>
    </TR>
    <TR>
      <TD><INPUT TYPE="radio" NAME="choix" VALUE="3"></TD>
      <TD>la répartition des appelants</TD>
    </TR>
	<TR>
      <TD><INPUT TYPE="radio" NAME="choix" VALUE="4"></TD>
      <TD>la répartition des appels par jour et par type</TD>
    </TR>
	
	
  </TABLE>

  <BR>

  <TABLE align="center">
    <TR>
      <TD>Entre le :</TD>
      <TD><INPUT type="text" size="10" name="date1" VALUE="$date1"></TD>
      <TD>JJ/MM/AAAA&nbsp;&nbsp;&nbsp;<IMG src="images/calendrier.jpg" align="middle" onMouseUp="javascript:showCal('Calendrier4')"></TD>
    </TR>
    <TR>
      <TD>Et le :</TD>
      <TD><INPUT type="text" size="10" name="date2" VALUE="$date2"></TD>
      <TD>JJ/MM/AAAA&nbsp;&nbsp;&nbsp;<IMG src="images/calendrier.jpg" align="middle" onMouseUp="javascript:showCal('Calendrier5')"></TD>
    </TR>
  </TABLE>

  <BR>

  <CENTER>
    <INPUT TYPE="submit" VALUE="Valider">
  </CENTER>

  </FORM>

END;

  return(array($html, null)) ;
}



//=======================================================
// PROGRAMME PRINCIPAL
//=======================================================

function states_switch($action)
{
  global $conn ;
  global $_REQUEST ;
  global $action ;
  global $redirect ;
  global $footer_msg ;


  switch ($action)
  {
    //-------------------------------------------------------------------------------------------
    case "states":
      // on affiche le formulaire de choix de statistiques
      $res = states_formulaire ($_REQUEST["choix"], $_REQUEST["date1"], $_REQUEST["date2"]) ;
      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        print ($res[0]) ;
      }
      break ;
    //-------------------------------------------------------------------------------------------
    case "states.choix":
      // on gère les différents choix de statistiques
      //cas numero 1
      if ($_REQUEST["choix"]==1)
      {
        // on affiche les statistiques numéro 1
        $res = states_choix1($_REQUEST["date1"], $_REQUEST["date2"]) ;
        if($res[1] != null)
        {
          print error($res[1]);
        }
        else
        {
          print ($res[0]);
        }
      }
      //cas numero 2
      if ($_REQUEST["choix"]==2)
      {
        // on affiche les statistiques numéro 2
        $res = states_choix2($_REQUEST["date1"], $_REQUEST["date2"]) ;
        if($res[1] != null)
        {
          print error($res[1]);
        }
        else
        {
          print ($res[0]);
        }
      }
      //cas numero 3
      if ($_REQUEST["choix"]==3)
      {
        // on affiche les statistiques numéro 3
        $res = states_choix3($_REQUEST["date1"], $_REQUEST["date2"]) ;
        if($res[1] != null)
        {
          print error($res[1]);
        }
        else
        {
          print ($res[0]);
        }
      }
      //cas numero 4
      if ($_REQUEST["choix"]==4)
      {
        // on affiche les statistiques numéro 4
        $res = states_choix4($_REQUEST["date1"], $_REQUEST["date2"]) ;
        if($res[1] != null)
        {
          print error($res[1]);
        }
        else
        {
          print ($res[0]);
        }
      }
	  
      break ;
    //-------------------------------------------------------------------------------------------
    default:
    $action="states" ;
  }
}

?>