<?php

//============================================================
// FONCTIONS
//============================================================


//------------------------------------------------------------------
// Fonction de modification d'un appel

function modifie ($date, $heure, $structure, $lieu, $appelant, $humeur, $type, $destinataire, $issue, $notif, $numero, $commentaire, $idappel, $utilisateur, $date1, $date2)
{
  global $conn ;

  // gestion des dates au format de la base
  $date_split = explode('/',$date) ;
  $time_split = explode(':',$heure) ;
  $date_time = mktime ($time_split[0], $time_split[1], $time_split[2], $date_split[1], $date_split[0], $date_split[2]) ;


  //=====================================================================
  // table LIEU
  //=====================================================================

  $sql = "SELECT lieu.nom, lieu.id FROM lieu WHERE nom = '".$lieu."'" ;
  $res = $conn->Execute($sql);

  if (!$res)
  {
    print "Error 1: " . $conn->ErrorMsg();
    die;
  }

  // gère les erreurs
  while (!$res->EOF)
  {
    $lieu_id = $res->fields[1];
    $res->MoveNext();
  }

  // on n'insère un nouveau lieu dans la base que s'il n'existe pas
  if ($lieu_id==NULL)
  {
    $sql = "INSERT INTO lieu ( nom ) VALUES ( " . $conn->qstr($lieu) . " ) " ;
    $res = $conn->Execute($sql);
    if (!$res)
    {
      print "Error 3: " . $conn->ErrorMsg();
      die;
    }
    $lieu_id = $conn->Insert_ID();
  }


  //=====================================================================
  // Table PERSONNE
  //=====================================================================

  $sql = "SELECT nom, id FROM personne WHERE nom = ". $conn->qstr($appelant);
  $res = $conn->Execute($sql);

  if (!$res)
  {
    print "Error 4: " . $conn->ErrorMsg();
    die;
  }

  // gère les erreurs
  while (!$res->EOF)
  {
    $personne_id = $res->fields[1];
    $res->MoveNext();
  }

  // on n'insère une nouvelle personne dans la base que si elle n'existe pas
  if ($personne_id==NULL)
  {
    $sql = "INSERT INTO personne ( nom ) VALUES ( " . $conn->qstr($appelant)." )" ;
    $res = $conn->Execute($sql);

    if (!$res)
    {
      print "Error 6: " . $conn->ErrorMsg();
      die;
    }
    $personne_id = $conn->Insert_ID();
  }


  //=====================================================================
  // table APPEL
  //=====================================================================

  $sql =  "UPDATE appel SET date_appel=".$conn->DBTimeStamp($date_time).", ".
                           "envoimail='".$notif."', ".
                           "commentaire=".$conn->qstr($commentaire).", ".
                           "tel=".$conn->qstr($numero).", ".
                           "id_humeur=".$humeur.", ".
                           "id_structure=".$structure.", ".
                           "id_issue=".$issue.", ".
                           "id_destinataire=".$destinataire.", ".
                           "id_type=".$type.", ".
                           "id_personne=".$personne_id.", ".
                           "id_lieu=".$lieu_id.", ".
                           "id_utilisateur='".$utilisateur."'".
          " WHERE id = ".$idappel ;

  $res = $conn->Execute($sql);

  if (!$res)
  {
    print "Error 7: " . $conn->ErrorMsg();
    die;
  }
  return $res ;
}

?>
