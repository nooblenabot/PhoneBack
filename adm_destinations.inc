<?php
//============GESTION DES DESTINATIONS D'APPELS PAR DEFAUT==========

//------------------------------------------------------------------
//    Fonction qui affiche tous les destinataires d'appels par défaut

function affiche_liste_destinations()
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;
  $barre_bouton = Barre_de_boutons_BO("","","Retour");  

  $sql = "SELECT da.id_type, da.id_destinataire, t.libelle, d.nom, d.prenom FROM destination_appel_def da, destinataire d, type t WHERE da.id_type = t.id AND da.id_destinataire = d.id ORDER BY 1,2" ;
  $res = $conn->Execute($sql) ;
  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  else
  {
    echo "<CENTER>" ;
    echo "<H2>Destinataires des appels par d&eacute;faut :</H2>" ;
    echo "<A HREF=\"index.php?action=administrer.affiche_formulaire_destination\">Ajouter</A>" ;
    echo "<BR><BR>" ;
    echo "<TABLE class=\"RESULTAT\"><TR><TH>Type d'appel</TH><TH>Destinataire</TH><TH colspan=\"2\">Actions</TH></TR>" ;
    while (!$res->EOF)
    {
      echo "<TR>
              <TD>".$res->fields[2]."</TD><TD>".$res->fields[4]." ".$res->fields[3]."</TD><TD>&nbsp;</TD>
              <TD align=center><A HREF=index.php?action=administrer.supprime_destination&idtype=".$res->fields[0]."><IMG SRC=\"images/delete.gif\" border=0></A></TD>
            </TR>" ;
      $res->MoveNext() ;
    }
    echo "</TABLE><br><br>" ;    
	echo $barre_bouton;
    echo "</CENTER>" ;
  }

}



//------------------------------------------------------------------
//    Fonction qui affiche le formulaire de saisie
//    des données de destination des appels par défaut

function administration_ajoutdestination_formulaire($idtype="", $iddestinataire="")
{

  $barre_bouton = Barre_de_boutons_BO("Insérer","Effacer","Retour");
  $menu_html1 = ListeDeChoix("SELECT libelle, id, '' FROM type ORDER BY libelle ASC","idtype", $idtype) ;
  $menu_html2 = ListeDeChoix("SELECT concat(prenom,' ',nom), id, '' FROM destinataire ORDER BY 1 ASC","iddestinataire", $iddestinataire) ;

  $html = <<<END

  <SCRIPT LANGUAGE="JAVASCRIPT">
    function verifie_formulaire()
    {
      return true ;
    }
  </SCRIPT>


    <FORM method="GET" name="infoDest" onSubmit="return verifie_formulaire()">
     <INPUT TYPE="hidden" NAME="action" VALUE="administrer.insere_destination">
      <H2>Veuillez saisir ou modifier les informations utilisateur :</H2>

      <TABLE class="RESULTAT" align="center">
        <TR>
          <TD>Type d'appel</TD>
          <TD>$menu_html1</TD>
        </TR>
        <TR>
          <TD>Destinataire par d&eacute;faut</TD>
          <TD>$menu_html2</TD>
        </TR>
      </TABLE>
      <BR>
      <CENTER>
		$barre_bouton
      </CENTER>
    </FORM>
END;

return(array($html, null)) ;
}


//------------------------------------------------------------------
//    Fonction qui insère la nouvelle destination d'appel par défaut

function insere_destination($p_idtype,$p_iddestinataire)
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;

  $sql = "DELETE from destination_appel_def WHERE id_type = ". $p_idtype;
  $res = $conn->Execute($sql) ;

  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  
  $sql = "INSERT INTO destination_appel_def ( id_type, id_destinataire ) ".
          "VALUES (".$conn->qstr($p_idtype).",".$conn->qstr($p_iddestinataire).")" ;
  $res = $conn->Execute($sql) ;

  if ($res == true)
  {
    $footer_msg = "Une destination d'appel par d&eacute;faut a bien &eacute;t&eacute; ins&eacute;r&eacute;e dans la base" ;
  }
  else
  {
    $footer_msg = "Il y a eu un probl&egrave;me lors de l'insertion dans la base" ;
  }
  $redirect = true ;
  $action = "administrer.choix" ;
}



//------------------------------------------------------------------
//    Fonction qui détruit une destination d'appel par défaut

function supprime_destination()
{
  global $conn ;
  global $redirect ;
  global $action ;
  global $footer_msg ;

  $sql = "DELETE FROM destination_appel_def WHERE id_type=".$_REQUEST["idtype"] ;
  $res = $conn->Execute($sql) ;
  if($conn->ErrorNo() != 0)
  {
    return ($conn->ErrorMsg()) ;
  }
  if ($res==true)
  {
    $footer_msg = "La destination d'appel par d&eacute;faut a bien &eacute;te d&eacute;truite de la base" ;
  }
  else
  {
    $footer_msg = "Il y a eu un probl&eagrave;me lors de la suppression." ;
  }
  $redirect = true ;
  $action = "administrer.choix" ;
}

?>