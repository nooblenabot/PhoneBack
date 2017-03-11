<?php

//============================================================
// FONCTION
//============================================================


//------------------------------------------------------------------
// Fonction d'affichage d'une fiche d'appel

function afficher_appel()
{
  global $conn ;
  global $_REQUEST ;
  global $action ;
  global $redirect ;
  global $footer_msg ;

  $date1 = $_REQUEST["date1"] ;
  $date2 = $_REQUEST["date2"] ;

  // pour que le lien retour ramène selon les cas
  // - à la page de saisie
  // - à la liste d'appels
  $provenance=$_REQUEST["provenance"] ;
  if ($provenance=="saisie")
  {
    $action="saisir" ;
  }
  elseif ($provenance=="consultation")
  {
    $action="consulter" ;
  }
  else
  {
    $action="consulter.liste" ;
  }

  $idAppel = $_REQUEST["idAppel"] ;

  $sql =  "SELECT a.date_appel, a.envoimail, a.commentaire, a.tel, s.nom, l.nom, p.nom, ".
                  "h.libelle, d.prenom, t.libelle, i.libelle ".
          "FROM appel a, structure s, lieu l, personne p, humeur h, destinataire d, type t, issue i ".
          "WHERE a.id =".$_REQUEST["idAppel"]."  AND s.id = a.id_structure AND ".
              "l.id = a.id_lieu AND p.id = a.id_personne AND ".
              "h.id = a.id_humeur AND d.id = a.id_destinataire AND ".
              "t.id = a.id_type AND i.id = a.id_issue " ;
  $res = $conn->Execute($sql) ;

  if (!$res)
  {
    print "Error 1: " . $conn->ErrorMsg() ;
    die;
  }

  echo "<BR>" ;
    
    // on récupère les résultats de la requête
    $timestamp = $conn->UnixTimeStamp($res->fields[0]) ;
    $date = date (("d/m/Y"),$timestamp) ;
    $heure = date (("H:i"),$timestamp) ;
    $structure = $res->fields[4];
    $lieu = $res->fields[5];
    $appelant = $res->fields[6];
    $humeur = $res->fields[7];
    $destinataire = $res->fields[8];
    $type = $res->fields[9];
    $issue = $res->fields[10];
    $notif = $res->fields[1];
    if ($notif!="oui")
    {
      $notif="non" ;
    }
    $numero = $res->fields[3] ;
    $commentaire = $res->fields[2] ;


    $html = <<<END

      <TABLE class="RESULTAT" align="center">
        <TR>
          <TH>Date</TH>
          <TD>&nbsp; $date</TD>
        </TR>
        <TR>
          <TH>Heure</TH>
          <TD>&nbsp; $heure</TD>
        </TR>
        <TR>
          <TH>Structure appelante</TH>
          <TD>&nbsp; $structure</TD>
        </TR>
        <TR>
          <TH>Localisation</TH>
          <TD>&nbsp; $lieu</TD>
        </TR>
        <TR>
          <TH>Appelant</TH>
          <TD>&nbsp; $appelant</TD>
        </TR>
        <!--
        <TR>
          <TH>Humeur appelant</TH>
          <TD>&nbsp; $humeur</TD>
        </TR>
        -->
        <TR>
          <TH>Destinataire</TH>
          <TD>&nbsp; $destinataire</TD>
        </TR>
        <TR>
          <TH>Type d'appel</TH>
          <TD>&nbsp; $type</TD>
        </TR>
        <TR>
          <TH>Issue de l'appel</TH>
          <TD>&nbsp; $issue</TD>
        </TR>
        <TR>
          <TH>Notification par m&eacute;l du destinataire</TH>
          <TD>&nbsp; $notif</TD>
        </TR>
        <TR>
          <TH>Num&eacute;ro de l'appelant</TH>
          <TD>&nbsp; $numero</TD>
        </TR>
        <TR>
          <TH>Commentaire</TH>
          <TD>&nbsp; $commentaire</TD>
        </TR>
      </TABLE>
      <BR>
      <CENTER>
        <A HREF="index.php?action=$action&date1=$date1&date2=$date2">Retour</A>&nbsp;|&nbsp;
        <A HREF="index.php?action=saisir.modifier&idAppel=$idAppel&date1=$date1&date2=$date2&provenance=$provenance">Modifier</A>
      </CENTER>
      <BR><BR>
END;

  return(array($html, null));
}

?>


