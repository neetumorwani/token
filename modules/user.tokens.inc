<?php
// $Id$

/**
 * @file
 * Builds placeholder replacement tokens for user-related data.
 */

/**
 * Implement hook_tokens().
 */
function user_tokens($type, $tokens, $data, $language = NULL, $sanitize = TRUE) {
  global $user;
  $url_options = array('absolute' => TRUE);
  if (isset($language)) {
    $url_options['language'] = $language;
  }

  $replacements = array();

  if ($type == 'user') {
    if (!empty($data['user'])) {
      $account = $data['user'];
    }
    else {
      $account = $user;
    }

    foreach ($tokens as $name => $original) {
      switch ($name) {
        // Basic user account information.
        case 'uid':
          $replacements[$original] = $account->uid;
          break;

        case 'name':
          $replacements[$original] = $sanitize ? filter_xss($account->name) : $account->name;
          break;

        case 'mail':
          $replacements[$original] = $sanitize ? filter_xss($account->mail) : $account->mail;
          break;

        case 'new-password':
          $replacements[$original] = $account->password;
          break;

        case 'url':
          $replacements[$original] = url("user/$account->uid", $url_options);
          break;

        case 'edit-url':
          $replacements[$original] = url("user/$account->uid/edit", $url_options);
          break;

        case 'confirm-cancel-url':
          $replacements[$original] = user_cancel_url($account);
          break;

        case 'password-reset-url':
          $replacements[$original] = user_pass_reset_url($account);
          break;

        // These tokens are default variations on the chained tokens handled below.
        case 'last-login':
          $replacements[$original] = format_date($account->login, 'medium', '', NULL, (isset($language) ? $language->language : NULL));
          break;

        case 'created':
          $replacements[$original] = format_date($account->created, 'medium', '', NULL, (isset($language) ? $language->language : NULL));
          break;
      }
    }

    if ($login_tokens = token_find_with_prefix($tokens, 'last-login')) {
      $replacements += module_invoke_all('tokens', 'date', $login_tokens, array('date' => $account->login), $language, $sanitize);
    }

    if ($registered_tokens = token_find_with_prefix($tokens, 'created')) {
      $replacements += module_invoke_all('tokens', 'date', $registered_tokens, array('date' => $account->created), $language, $sanitize);
    }
  }

  return $replacements;
}


function user_token_info() {
  $results = array();

  $type['name'] = t('Users');
  $type['description'] = t('Tokens related to individual user accounts.');


  $user['uid']['name'] = t('User ID');
  $user['uid']['description'] = t('The unique ID of the user account.');

  $user['name']['name'] = t('Name');
  $user['name']['description'] = t("The login name of the user account.");

  $user['mail']['name'] = t('Email');
  $user['mail']['description'] = t('The email address of the user account.');

  $user['new-password']['name'] = t('New password');
  $user['new-password']['description'] = t("A newly-assigned password given to the user account on creation.");

  $user['url']['name'] = t('URL');
  $user['url']['description'] = t('The url of the account profile page.');

  $user['edit-url']['name'] = t('Edit URL');
  $user['edit-url']['description'] = t('The url of the account edit page.');

  $user['confirm-cancel-url']['name'] = t('Account cancellation URL');
  $user['confirm-cancel-url']['description'] = t("The account cancellation URL for the user account. Visiting this URL will automatically cancel a user account: it cannot be undone.");

  $user['password-reset-url']['name'] = t('Password reset URL');
  $user['password-reset-url']['description'] = t('A single-use password reset URL for the user account.');

  $user['last-login']['name'] = t('Last login');
  $user['last-login']['description'] = t('The date the user last logged in to the site.');
  $user['last-login']['references'] = 'date';

  $user['created']['name'] = t('Created');
  $user['created']['description'] = t('The date the user account was created.');
  $user['created']['references'] = 'date';


  $results['types']['user'] = $type;
  $results['tokens']['user'] = $user;
  return $results;
}