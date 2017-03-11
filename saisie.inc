<?php

//*************************************************************************************************
// SOUS-CONTROLEUR DE LA SAISIE                                                                   *
// + fonction d'affichage du formulaire de saisie                                                 *
//*************************************************************************************************



include "insert.inc" ;
include "modifie.inc" ;
include "fiche_appel.inc" ;


//============================================================
// FONCTIONS
//============================================================


//------------------------------------------------------------------
// Fonction d'affichage du formulaire de saisie

function saisie_formulaire ($date = "", $heure="", $structure = "", $lieu = "", $appelant = "", $humeur = "", $type = "", $destinataire = "", $issue = "", $notif="", $numero="", $commentaire="", $idAppel="", $provenance="" )
{
  global $conn ;

  $date1 = $_REQUEST["date1"] ;
  $date2 = $_REQUEST["date2"] ;

  // génération de la date et de l'heure
  if ($date!="")
  {
    $datecourante = $date ;
    $heurecourante = $heure ;
  }
  else
  {
    // on récupère la date courante
    $datecourante = date (("d/m/Y"),time()) ;
    // on récupère l'heure courante
    $heurecourante = date (("H:i"),time()) ;
  }

  // génération de la selectbox de STRUCTURE APPELANTE
  $menu_html = ListeDeChoix("SELECT nom, id, defaut FROM structure ORDER BY id ASC","structure", $structure) ;
  // génération de la selectbox de HUMEUR DE L'APPELANT
    // ci-dessous : si on veut gérer l'humeur
    $menu2_html = BoutonsRadio("SELECT libelle, id, defaut, smiley FROM humeur ORDER BY id ASC","humeur", $humeur, 'smiley') ;
  //$menu2_html = "<INPUT type=\"hidden\" name=\"humeur\" value=\"3\">";
  // génération de la selectbox de TYPE D'APPEL
  $menu3_html = ListeDeChoix("SELECT libelle, id, defaut FROM type ORDER BY id ASC","type",$type, "onChange='javascript:synchro_destinataire(this.value);'") ;
  // génération de la selectbox de DESTINATAIRE
  $menu4_html = ListeDeChoix("SELECT prenom, id, defaut FROM destinataire ORDER BY id ASC","destinataire",$destinataire) ;
  // génération de la selectbox de ISSUE DE L'APPEL
  $menu5_html = ListeDeChoix("SELECT libelle, id, defaut FROM issue ORDER BY id ASC","issue",$issue) ;


  // pour permettre au formulaire de gérer soit l'insertion, soit la modification
  if ($idAppel=="")
  {
    $myaction="saisir.inserer";
  }
  else
  {
    $myaction="saisir.applimodifs";
  }


  // génération de la fonction javascript de synchronisation des destinataires par rapport aux types d'appel.

  $script_synchro = "function synchro_destinataire (selectedId)
                     {
                       destSB = window.document.forms[0].destinataire;
                       len = destSB.length;
                       destId = 0;\n";

  $sql =  "SELECT id_type, id_destinataire FROM destination_appel_def";
  $rs = $conn->Execute($sql); // Execute the query and get the empty recordset
  if (!$rs)
  {
    print $conn->ErrorMsg() ;
  }
  while (!$rs->EOF)
  {
    $script_synchro .= "        if (selectedId == ". $rs->fields[0] .") {destId = ". $rs->fields[1] .";}\n";
    $rs->MoveNext() ;
  }

  $script_synchro .= "          for (var i = 0; i < len; i++)
                                {
                                  if (destSB.options[i].value == destId)
                                  {
                                    destSB.options[i].selected = true
                                  }
                                }
                              }\n
                            ";
  
  $html = <<<END

      <SCRIPT LANGUAGE="JAVASCRIPT">
        // fonction de synchronisation des destinataires par rapport aux types d'appel.
        $script_synchro

        function reload_heure()
        {
          var Aujourdhui = new Date() ;
          var heure = Aujourdhui.getHours() ;
          var minute = Aujourdhui.getMinutes() ;

          // pour toujours obtenir une heure de format HH:MM et non parfois H:M
          if (heure<10)
          {
            var heuref = "0"+heure ;
          }
          else
          {
              var heuref = heure ;
          }
          if (minute<10)
          {
              var minutef = "0"+minute ;
          }
          else
          {
            var minutef = minute ;
          }
          window.document.forms["saisie"].elements["heure"].value = heuref + ":" +  minutef;
        }

        function verifie_formulaire()
        {
          var chpnum = window.document.forms["saisie"].elements["numero"].value ;
          var chpdate = window.document.forms["saisie"].elements["date"].value ;
          var chpheure = window.document.forms["saisie"].elements["heure"].value ;

          // test du champ numéro de téléphone
          if (  chpnum.length !=0
                && ( chpnum.charAt(2) != "."
                    || chpnum.charAt(5) != "."
                    || chpnum.charAt(8) != "."
                    || chpnum.charAt(11) != "."
                    || chpnum.length != 14
                    || isNaN(chpnum.substring(0,1))
                    || isNaN(chpnum.substring(3,4))
                    || isNaN(chpnum.substring(6,7))
                    || isNaN(chpnum.substring(6,7))
                    || isNaN(chpnum.substring(9,10))
                    || isNaN(chpnum.substring(12,13))
                  )
              )
          {
            alert("Attention, le numéro de téléphone n'est pas au bon format. XX.XX.XX.XX.XX");
            return false;
          }

          // test du champ date
          if ( chpdate.length != 10
                || (chpdate.charAt(2) != "/") || (chpdate.charAt(5) != "/")
                || isNaN(chpdate.substring(0,2))
                || isNaN(chpdate.substring(3,5))
                || isNaN(chpdate.substring(7,10))
              )
          {
            alert("Attention, le champ date n'est pas au bon format : JJ/MM/AAAA");
            return false;
          }

          //test du champ heure
          if (chpheure.length != 5
              || ( chpheure.charAt(2) != ":")
              || isNaN(chpheure.substring(0,2))
              || isNaN(chpheure.substring(3,5))
              )
          {
            alert("Attention, le champ heure n'est pas au bon format : XX:XX");
            return false;
         }

          else
          {
            return true;
          }
       }
      </SCRIPT>


      <!------------------------------------------------>
      <!-- JAVASCRIPTS nécessaires pour le calendrier -->
      <!------------------------------------------------>
      <script language="javascript" src="javascript/cal2.js"></script>
      <script language="javascript" src="javascript/cal_conf2.js"></script>


        <H1>Veuillez saisir le formulaire suivant :</H1>

        <FORM method="GET" NAME="saisie" onSubmit="return verifie_formulaire()">
        <INPUT TYPE="hidden" NAME="action" VALUE="$myaction">
        <INPUT TYPE="hidden" NAME="provenance" VALUE="$provenance">
        <INPUT TYPE="hidden" NAME="idAppel" VALUE="$idAppel">
        <INPUT TYPE="hidden" NAME="date1" VALUE="$date1">
        <INPUT TYPE="hidden" NAME="date2" VALUE="$date2">

        <TABLE class="RESULTAT" align="center">
                <TR>
                        <TH>Date</TH>
                        <TD><INPUT type="text" name="date" size="10" value="$datecourante">&nbsp;&nbsp;&nbsp;<IMG src="images/calendrier.jpg" align="middle" onMouseUp="javascript:showCal('Calendrier')"></TD>

                        <TH>Heure</TH>
                        <TD><INPUT type="text" name="heure" size="10" value="$heurecourante">&nbsp;&nbsp;<INPUT type="button" value="Actualiser" onClick="javascript:reload_heure()"></TD>
                </TR>
                <TR>
                        <TH>Structure appelante</TH>
                        <TD colspan="3"> $menu_html </TD>
                </TR>
                <TR>
                        <TH>Localisation</TH>
                        <TD><INPUT type="text" name="lieu" size="30" value="$lieu"></TD>

                        <TH>Appelant</TH>
                        <TD><INPUT type="text" name="appelant" size="30" value="$appelant"></TD>
                </TR>
                <!-- 
                <TR>
                        <TH>Humeur de l'appelant</TH>
                        <TD colspan="3"> $menu2_html </TD>
                </TR>
                -->
                <INPUT type=hidden name=humeur value='6'>
                
                <TR>
                        <TH>Type d'appel</TH>
                        <TD colspan="3"> $menu3_html </TD>
                </TR>
                <TR>
                        <TH>Destinataire</TH>
                        <TD colspan="3"> $menu4_html </TD>
                </TR>
                <TR>
                        <TH>Issue de l'appel</TH>
                        <TD> $menu5_html </TD>

                        <TH colspan="2">
                          <INPUT type="checkbox" name="notif" value="oui">Notifier par m&eacute;l le destinataire
                        </TH>
                </TR>
                <TR>
                        <TH>Num&eacute;ro de l'appelant</TH>
                        <TD colspan="3"><INPUT type="text" name="numero" size="14" value="$numero"></TD>
                </TR>
                <TR>
                        <TH>Commentaire</TH>
                        <TD colspan="3"><TEXTAREA name="commentaire" cols=50 rows=5>$commentaire</TEXTAREA></TD>
                </TR>
        </TABLE>

        <BR>

        <CENTER>
          <INPUT type="reset" value="Effacer">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
          <INPUT type="submit" value="Valider">
        </CENTER>
        </FORM>
        <BR>
        
END;

  return(array($html, null));
}



//------------------------------------------------------------------
// Fonction d'affichage des 5 derniers appels saisis

function saisie_affichederniersappels ()
{
  global $conn ;

  $sql =  "SELECT a.id, a.date_appel, s.nom, l.nom, p.nom ".
          "FROM appel a, structure s, lieu l, personne p ".
          "WHERE a.id_structure = s.id ".
            "AND a.id_lieu = l.id ".
            "AND a.id_personne = p.id ".
          "ORDER BY a.id DESC ".
          "LIMIT 5 " ;
          
  $rs = $conn->Execute($sql) ; // Execute the query and get the empty recordset
  $nb = $rs->RowCount("appel") ;
  if (!$rs)
  {
    print $conn->ErrorMsg() ;
  }
  else
  {
    echo "<H2>Les cinq derniers appels saisis...</H2>" ;
    // si on n'a pas de résultat à notre requête
    if (($nb) == NULL)
    {
      echo "<H2>Votre base est vide pour l'instant !</H2>" ;
    }
    else
    {
      echo "<TABLE class=\"RESULTAT\" align=\"center\">" ;
      echo "  <TR><TH>Date</TH><TH>Heure</TH><TH>Structure appelante</TH><TH>Localisation</TH><TH>Appelant</TH></TR>" ;
      while (!$rs->EOF)
      {
       $timestamp = $conn->UnixTimeStamp($rs->fields[1]) ;
        $date = date (("d/m/Y"),$timestamp) ;
        $heure = date (("H:i"),$timestamp) ;
        echo "<TR>" ;
        echo "  <TD>&nbsp;".$date."</TD><TD>&nbsp;".$heure."</TD>" ;
        echo "  <TD>&nbsp;".$rs->fields[2]."</TD><TD>&nbsp;".$rs->fields[3]."</TD><TD>&nbsp;".$rs->fields[4]."</TD>" ;
        echo "  <TD><A HREF=\"index.php?action=consulter.fiche&idAppel=".$rs->fields[0]."&date1=".isset($date1)."&date2=".isset($date2)."&provenance=saisie\">Plus d'infos</A></TD>" ;
        echo "  <TD><A HREF=\"index.php?action=saisir.modifier&idAppel=".$rs->fields[0]."&date1=".isset($date1)."&date2=".isset($date2)."&provenance=saisie\" >Modifier</A></TD>" ;
        $rs->MoveNext() ;
        echo "</TR>" ;
      }
      echo "</TABLE>" ;
      echo "<BR><BR>" ;
    }
  }
}




//=======================================================
// PROGRAMME PRINCIPAL
//=======================================================

function saisir_switch($action)
{
  global $action ;
  global $redirect ;
  global $footer_msg ;
  global $conn ;
  global $_REQUEST ;
  

  switch ($action)
  {
    //-------------------------------------------------------------------------------------------
    case "saisir":
      // on affiche le formulaire de saisie
      $res = saisie_formulaire();
      if($res[1] != null)
      {
        print error($res[1]);
      }
      else
      {
        print ($res[0]);
        $provenance ="saisie" ;
        saisie_affichederniersappels () ;
      }
      break;
    //-------------------------------------------------------------------------------------------
    case "saisir.inserer":
      // on insère l'appel
      $res = insert($_REQUEST["date"], $_REQUEST["heure"], $_REQUEST["structure"], $_REQUEST["lieu"], $_REQUEST["appelant"],
                    $_REQUEST["humeur"], $_REQUEST["type"], $_REQUEST["destinataire"], $_REQUEST["issue"], $_REQUEST["notif"],
                    $_REQUEST["numero"], $_REQUEST["commentaire"]);
      if ($res != true )
      {
        $action = "saisir" ;
        $footer_msg = "Il y a eu un probl&egrave;me lors de la transmission des donn&eacute;es, veuillez recommencer la saisie" ;
      }
      else
      {
        $action = "saisir" ;
        $footer_msg = "Les donn&eacute;es ont &eacute;t&eacute; correctement enregistr&eacute;es dans la base." ;
      }
      $redirect = true ;
      break;
    //-------------------------------------------------------------------------------------------
    case "saisir.modifier":
      $sql =  "SELECT appel.id, date_appel, structure.id, lieu.nom, personne.nom, humeur.id, " .
                "type.id, destinataire.id, issue.id, appel.envoimail, appel.tel, appel.commentaire " .
              "FROM appel, structure, personne, lieu, humeur, destinataire, type, issue ".
              "WHERE personne.id = appel.id_personne AND " .
                "structure.id = appel.id_structure AND " .
                "personne.id = appel.id_personne AND " .
                "lieu.id = appel.id_lieu AND " .
                "humeur.id = appel.id_humeur AND " .
                "destinataire.id = appel.id_destinataire AND " .
                "type.id = appel.id_type AND " .
                "issue.id = appel.id_issue AND " .
                "appel.id = ".$_REQUEST["idAppel"] ;    // condition principale
      $rs=$conn->Execute($sql) ;

      $timestamp = $conn->UnixTimeStamp($rs->fields[1]) ;
      $date = date (("d/m/Y"),$timestamp) ;
      $heure = date (("H:i"),$timestamp) ;

      // on affiche le formulaire de saisie, rempli avec les différents paramètres
      // arguments nécessaires pour la fonction formulaire :
      //                        date| heure |   structure   |    lieu      |   appelant   |   humeur     |    type      | destinataire |   issue      | notif        |     numero     |   commentaire |     idAppel  |    pour gérer le lien de retour (dirige vers des endroits différents selon la provenance)
      $res = saisie_formulaire($date, $heure, $rs->fields[2],$rs->fields[3],$rs->fields[4],$rs->fields[5],$rs->fields[6],$rs->fields[7],$rs->fields[8],$rs->fields[9], $rs->fields[10],$rs->fields[11],$rs->fields[0], $_REQUEST["provenance"]) ;

      if($res[1] != null)
      {
        print error($res[1]) ;
      }
      else
      {
        print ($res[0]);
      }
      break ;
    //-------------------------------------------------------------------------------------------
    case "saisir.applimodifs":
      // on modifie l'appel
      $res = modifie ($_REQUEST["date"], $_REQUEST["heure"], $_REQUEST["structure"], $_REQUEST["lieu"], $_REQUEST["appelant"],
                    $_REQUEST["humeur"], $_REQUEST["type"], $_REQUEST["destinataire"], $_REQUEST["issue"], isset($_REQUEST["notif"]),
                    $_REQUEST["numero"], $_REQUEST["commentaire"], $_REQUEST["idAppel"], $_SESSION["id_utilisateur"],
                    $_REQUEST["date1"], $_REQUEST["date2"], $_REQUEST["provenance"] );

      if (($res) == true)
      {
        // selon d'où l'on vient, on a un lien de retour différent
        if ($_REQUEST["provenance"] == "saisie"  )
        {
          // retour vers la première page de saisie
          $action = "saisir" ;
        }
        elseif ($_REQUEST["provenance"] == "consultation" )
        {
          // retour vers la première page de consultation
          $action= "consulter" ;
        }
        else
        {
          // retour vers la liste de consultation
          $action = "consulter.liste" ;
        }
        $date1= $_REQUEST["date1"] ;
        $date2= $_REQUEST["date2"] ;
        $footer_msg = "Les donn&eacute;es ont bien &eacute;t&eacute; modifi&eacute;es." ;
      }
      else
      {
        $action = "saisir.modifier" ;
        $footer_msg = "Vos donn&eacute;es n'ont pas &eacute;t&eacute; modifi&eacute;es, veuillez recommencer" ;
      }
      $redirect = true ;
      break;
    //-------------------------------------------------------------------------------------------
    default:
    $action="saisir";
 }
}

?>

