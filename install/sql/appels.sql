# phpMyAdmin SQL Dump
# version 2.5.3
# http://www.phpmyadmin.net
#
# Serveur: localhost
# Généré le : Mardi 06 Juin 2006 à 16:48
# Version du serveur: 4.0.15
# Version de PHP: 4.3.3
# 
# Base de données: `appels`
# 

# --------------------------------------------------------

#
# Structure de la table `appel`
#
# Création: Lundi 29 Mai 2006 à 12:32
# Dernière modification: Jeudi 01 Juin 2006 à 15:55
#

CREATE TABLE `appel` (
  `id` int(10) NOT NULL auto_increment,
  `date_appel` datetime NOT NULL default '0000-00-00 00:00:00',
  `envoimail` varchar(5) default NULL,
  `commentaire` text,
  `tel` varchar(14) default NULL,
  `id_humeur` int(10) NOT NULL default '0',
  `id_lieu` int(10) default NULL,
  `id_structure` int(10) NOT NULL default '0',
  `id_issue` int(10) NOT NULL default '0',
  `id_destinataire` int(10) NOT NULL default '0',
  `id_type` int(10) NOT NULL default '0',
  `id_utilisateur` int(10) NOT NULL default '0',
  `id_personne` int(10) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8394 ;

#
# Contenu de la table `appel`
#


# --------------------------------------------------------

#
# Structure de la table `destination_appel_def`
#
# Création: Lundi 29 Mai 2006 à 12:32
# Dernière modification: Mardi 06 Juin 2006 à 12:08
#

CREATE TABLE `destination_appel_def` (
  `id_destinataire` int(10) NOT NULL default '0',
  `id_type` int(10) NOT NULL default '0'
) ENGINE=MyISAM;

#
# Contenu de la table `destination_appel_def`
#

INSERT INTO `destination_appel_def` (`id_destinataire`, `id_type`) VALUES (285, 1),
(284, 26),
(284, 32),
(1, 4),
(286, 5);

# --------------------------------------------------------

#
# Structure de la table `droits_acces`
#
# Création: Mardi 30 Mai 2006 à 14:46
# Dernière modification: Jeudi 01 Juin 2006 à 16:01
#

CREATE TABLE `droits_acces` (
  `id` int(11) NOT NULL auto_increment,
  `idrubrique` int(11) NOT NULL default '0',
  `idutilisateur` int(11) NOT NULL default '0',
  `acces` char(3) NOT NULL default 'oui',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM COMMENT='Gestion d''un droit d''accès (oui ou non) par rubrique et par ' AUTO_INCREMENT=49 ;

#
# Contenu de la table `droits_acces`
#

INSERT INTO `droits_acces` (`id`, `idrubrique`, `idutilisateur`, `acces`) VALUES (21, 1, 6, 'oui'),
(22, 2, 6, 'oui'),
(23, 3, 6, 'oui'),
(24, 4, 6, 'oui');

# --------------------------------------------------------

#
# Structure de la table `humeur`
#
# Création: Lundi 29 Mai 2006 à 12:32
# Dernière modification: Lundi 29 Mai 2006 à 12:33
#

CREATE TABLE `humeur` (
  `id` int(10) NOT NULL auto_increment,
  `libelle` varchar(20) NOT NULL default '',
  `description` varchar(30) default NULL,
  `smiley` varchar(50) NOT NULL default '',
  `defaut` char(3) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 ;

#
# Contenu de la table `humeur`
#

INSERT INTO `humeur` (`id`, `libelle`, `description`, `smiley`, `defaut`) VALUES (1, 'Mauvaise', NULL, 'images/icon_mauvaise.gif', NULL),
(2, 'Moyenne', NULL, 'images/icon_moyenne.gif', NULL),
(3, 'Normale', NULL, 'images/icon_normale.gif', NULL),
(4, 'Bonne', NULL, 'images/icon_bonne.gif', NULL),
(5, 'Excellente', NULL, 'images/icon_excellente.gif', NULL),
(6, 'Ne se prononce pas', NULL, 'images/icon_question.gif', 'oui');

# --------------------------------------------------------

#
# Structure de la table `issue`
#
# Création: Lundi 29 Mai 2006 à 12:32
# Dernière modification: Mardi 06 Juin 2006 à 16:35
#

CREATE TABLE `issue` (
  `id` int(10) NOT NULL auto_increment,
  `libelle` varchar(30) NOT NULL default '',
  `description` varchar(30) default NULL,
  `defaut` char(3) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=20 ;

#
# Contenu de la table `issue`
#

INSERT INTO `issue` (`id`, `libelle`, `description`, `defaut`) VALUES (1, 'Appel pris', '', 'oui'),
(2, 'Il rappellera', NULL, 'non'),
(3, 'Le rappeler', NULL, 'non'),
(4, 'Autre', NULL, 'non'),
(5, 'Il téléphone sur portable', NULL, 'non'),
(6, 'Il envoie un mail', NULL, 'non');

# --------------------------------------------------------

#
# Structure de la table `lieu`
#
# Création: Mardi 06 Juin 2006 à 15:35
# Dernière modification: Mardi 06 Juin 2006 à 15:35
#

CREATE TABLE `lieu` (
  `id` int(10) NOT NULL auto_increment,
  `nom` varchar(50) NOT NULL default '',
  `codepostal` int(5) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 ;

#
# Contenu de la table `lieu`
#


# --------------------------------------------------------

#
# Structure de la table `personne`
#
# Création: Mardi 06 Juin 2006 à 15:33
# Dernière modification: Mardi 06 Juin 2006 à 15:33
#

CREATE TABLE `personne` (
  `id` int(10) NOT NULL auto_increment,
  `nom` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 ;

#
# Contenu de la table `personne`
#


# --------------------------------------------------------

#
# Structure de la table `rubrique`
#
# Création: Mardi 30 Mai 2006 à 14:49
# Dernière modification: Mardi 30 Mai 2006 à 14:51
#

CREATE TABLE `rubrique` (
  `id` int(11) NOT NULL auto_increment,
  `lib` varchar(255) NOT NULL default '',
  `ordre_affichage` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 ;

#
# Contenu de la table `rubrique`
#

INSERT INTO `rubrique` (`id`, `lib`, `ordre_affichage`) VALUES (1, 'SAISIE', 1),
(2, 'CONSULTATION', 2),
(3, 'STATISTIQUES', 3),
(4, 'ADMINISTRATION', 4);

# --------------------------------------------------------

#
# Structure de la table `structure`
#
# Création: Lundi 29 Mai 2006 à 14:56
# Dernière modification: Mardi 06 Juin 2006 à 12:21
#

CREATE TABLE `structure` (
  `id` int(10) NOT NULL auto_increment,
  `nom` varchar(50) NOT NULL default '',
  `defaut` char(3) default 'Non',
  `Client` char(3) default 'Oui',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=46 ;

#
# Contenu de la table `structure`
#

INSERT INTO `structure` (`id`, `nom`, `defaut`, `client`) VALUES (17, 'Collège', 'non', 'oui'),
(45, 'Service interne', 'oui', 'oui'),
(20, 'Autre', 'non', 'oui'),
(11, 'Particulier', 'non', 'oui');

# --------------------------------------------------------

#
# Structure de la table `destinataire`
#
# Création: Mardi 06 Juin 2006 à 10:51
# Dernière modification: Mardi 06 Juin 2006 à 12:07
#

CREATE TABLE `destinataire` (
  `id` int(10) NOT NULL auto_increment,
  `nom` varchar(30) NOT NULL default '',
  `prenom` varchar(30) NOT NULL default '',
  `mail` varchar(50) NOT NULL default '',
  `tel` varchar(14) default NULL,
  `id_structure` int(10) NOT NULL,
  `defaut` char(3) default 'non',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=287 ;

#
# Contenu de la table `destinataire`
#

INSERT INTO `destinataire` (`id`, `nom`, `prenom`, `mail`, `tel`,`id_structure`, `defaut`) VALUES (284, 'Durand', 'Robert', 'rdurand@mondomaine.com', 'Poste 327',17, 'non'),
(285, 'Dufour', 'Josiane', 'jdufour@mondomaine.com', '7997',11, 'non'),
(286, 'Duchmol', 'Jean Jacques', 'jduch@mondomaine.com', '35-55',20, 'oui');

# --------------------------------------------------------

#
# Structure de la table `type`
#
# Création: Lundi 29 Mai 2006 à 12:33
# Dernière modification: Mardi 06 Juin 2006 à 16:21
#

CREATE TABLE `type` (
  `id` int(10) NOT NULL auto_increment,
  `libelle` varchar(30) NOT NULL default '',
  `description` varchar(30) default NULL,
  `defaut` char(3) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=35 ;

#
# Contenu de la table `type`
#

INSERT INTO `type` (`id`, `libelle`, `description`, `defaut`) VALUES (1, 'Support', '', 'oui'),
(5, 'Comptabilité', '', 'non'),
(26, 'Autre', '', 'non'),
(34, 'Démarche commerciale', '', 'non');

# --------------------------------------------------------

#
# Structure de la table `utilisateur`
#
# Création: Lundi 29 Mai 2006 à 12:33
# Dernière modification: Jeudi 01 Juin 2006 à 16:01
#

CREATE TABLE `utilisateur` (
  `id` int(10) NOT NULL auto_increment,
  `nom` varchar(30) NOT NULL default '',
  `prenom` varchar(30) NOT NULL default '',
  `login` varchar(20) NOT NULL default '',
  `motpasse` varchar(10) NOT NULL default '',
  `mail` varchar(50) NOT NULL default '',
  `Profil` int(10) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=24 ;

#
# Contenu de la table `utilisateur`
#

INSERT INTO `utilisateur` (`id`, `nom`, `prenom`, `login`, `motpasse`, `mail`,`Profil`) VALUES (6, 'ADMINISTRATEUR', 'admin', 'admin', 'password', 'info@erasme.org','1');
