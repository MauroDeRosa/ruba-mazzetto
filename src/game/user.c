#include <game/user.h>
#include <types/array.h>
#include <types/vector.h>
#include <types/memory.h>
#include <io/log.h>
#include <utils/timeutils.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#define USER_TABLE_PATH "./users.bin" ///< the user table binary file path
#define USER_TABLE_DEFAULT_SIZE 20 ///< the user table default size (or how much the table size will increase if space's needed)
#define USERID_MAX UINT_MAX ///< maximum user size

vector *users;
// user *users->elements; ///< the user table
// size_t table_count; ///< the number of users in the table
// size_t table_size; ///< the size of the user table (the allocated memory is resized when needed)
FILE *table_file; ///< the user table file descriptor
size_t last_id; ///< last used id

userid *logged_table;
size_t logged_count;

int compare_userid(const void* a, const void* b)
{
    const userid *au = a, *bu = b;
    return *au - *bu;
}

bool user_is_logged(userid id)
{
    userid *found = binary_search(&id, logged_table, logged_count, sizeof(userid), compare_userid);
    return found != NULL;
}

bool user_is_valid(userid id)
{
    if(id == USERID_INVALID)
    {
        return false;
    }
    else if(is_userid_in_table(id) == false)
    {
        return false;
    }
    
    return true;
}

/**
 * @brief comparator used for qsort and binary_search
 */
int compare_by_username(const void *a, const void *b)
{
    const user *ua = a, *ub = b;
    return strcmp(ua->username, ub->username);
}

user* user_get_by_username(const char* username)
{
    if(username_is_valid(username) == false)
    {
        log_error("invalid username");
    }

    user value;
    strcpy(value.username, username);
    value.username[USERNAME_MAX_LEN] = '\0';
    return binary_search(&value, users->elements, users->count, sizeof(user), compare_by_username);
}

/**
 * @brief comparator used for qsort and binary_search
 */
int compare_by_id(const void *a, const void *b)
{
    const user *ua = a, *ub = b;
    return (int)(ua->id) - (int)(ub->id);
}

user* user_get_by_userid(userid id)
{
    user value = (user){.id = id};
    return binary_search(&value, users->elements, users->count, sizeof(user), compare_by_id);
}

bool is_userid_in_table(userid id)
{
    return user_get_by_userid(id) != NULL;
}

bool user_username_in_table(const char *username)
{
    return user_get_by_username(username) != NULL;
}

bool username_is_valid(const char *username)
{
    size_t username_len = strlen(username);

    if (username_len < USERNAME_MIN_LEN || username_len > USERNAME_MAX_LEN)
    {
        return false;
    }

    for (size_t i = 0; i < username_len; i++)
    {
        if (!isalnum(username[i]) && username[i] != '_')
        {
            return false;
        }
    }

    return true;
}

bool password_is_valid(const char *password)
{
    size_t password_len = strlen(password);

    if (password_len < PASSWORD_MIN_LEN || password_len > PASSWORD_MAX_LEN)
    {
        return false;
    }

    for (size_t i = 0; i < password_len; i++)
    {
        if (!isalnum(password[i]) && !ispunct(password[i]))
        {
            return false;
        }
    }

    return true;
}

size_t users_get_count()
{
    return users->count;
}

userid user_register(const char *username, const char *password)
{
    if (last_id == USERID_MAX)
    {
        // TODO: check for unused ids
        log_warning("user limit reached, no more users can be created");
        return USERID_INVALID;
    }
    
    if (username_is_valid(username) == false)
    {
        log_warning("invalid username");
        return USERID_INVALID;
    }

    if (password_is_valid(password) == false)
    {
        log_warning("invalid username");
        return USERID_INVALID;
    }


    if(user_username_in_table(username))
    {
        log_warning("user %s already exist", username);
        return USERID_INVALID;
    }

    time_t creation_time = time(NULL);
    user new_user;

    new_user.created = *localtime(&creation_time);
    new_user.modified = new_user.created;
    strcpy(new_user.username, username);
    strcpy(new_user.password, password);
    new_user.id = ++last_id;
    
    VEC_APPEND(user, users, new_user);
    users_save();
    return new_user.id;
}

userid user_update(userid id, const char *password)
{
    if(password_is_valid(password) == false)
    {
        log_warning("invalid password");
        return USERID_INVALID;
    }
    
    user *u = user_get_by_userid(id);

    if (u == NULL)
    {
        log_warning("user with id %zu doesn't exist", id);
        return USERID_INVALID;
    }    

    time_t time_now = time(NULL);
    u->modified = *localtime(&time_now);
    strcpy(u->password, password);
    u->password[PASSWORD_MAX_LEN] = '\0';
    users_save();
    return id;
}

userid user_login(const char *username, const char *password)
{
    if (username_is_valid(username) == false)
    {
        log_warning("invalid username");
        return USERID_INVALID;
    }

    if (password_is_valid(password) == false)
    {
        log_warning("invalid password");
        return USERID_INVALID;
    }

    user *u = user_get_by_username(username);
    
    if(u == NULL)
    {
        log_warning("user %s doesn't exist", username);
        return USERID_INVALID;
    }
    else if (user_is_logged(u->id))
    {
        log_warning("user %s is already logged in", u->username, u->id);
        return USERID_INVALID;
    }

    if(strcmp(u->password, password) == 0)
    {
        logged_table[logged_count++] = u->id;
        return u->id;
    }
    else
    {
        log_warning("wrong password");
        return USERID_INVALID;
    }
}

void user_logout(userid id)
{
    userid *found = binary_search(&id, logged_table, logged_count, sizeof(userid), compare_userid);
    if( found != NULL)
    {
        size_t index = array_index_from_ptr(logged_table, found, sizeof(userid));
        array_delete(index, logged_table, &logged_count, sizeof(userid));
    }
}

void user_delete(userid id)
{
    user *u = user_get_by_userid(id);

    if (u == NULL)
    {
        log_error("user with id %zu doesn't exist", id);
    }

    size_t index = array_index_from_ptr(users->elements, u, sizeof(user));
    VEC_DELETE(users, index);
    users_save();
}

void users_save()
{
    FILE *f = fopen(USER_TABLE_PATH, "wb");
    fwrite(&last_id, sizeof(userid), 1, f);
    fwrite(&last_id, sizeof(size_t), 1, f);
    fwrite(users->elements, users->element_size, users->count, f);
    fclose(f);
}

void user_load()
{
    FILE *f = fopen(USER_TABLE_PATH, "rb");
    fread(&users->count, sizeof(size_t), 1, f);
    fread(&last_id, sizeof(userid), 1, f);
    VEC_RESIZE(users, users->count);
    fread(users->elements, users->element_size, users->count, f);
    fclose(f);
}

void users_init()
{
    users = VEC_INIT(user);

    // file exists
    if(access(USER_TABLE_PATH, F_OK) == 0)
    {
        user_load();
    }
    else
    {
        last_id = USERID_INVALID;
    }

    logged_count = 0;
    logged_table = memory_allocate_typed(USER_TABLE_DEFAULT_SIZE, sizeof(user));
}

char* user_json(user *u)
{
    check_null_pointer(u);
    char* json = calloc(400, sizeof(char));
    char created[DATETIME_STR_SIZE] = {0}, modified[DATETIME_STR_SIZE] = {0};
    datetime_string(u->created, created);
    datetime_string(u->modified, modified);
    sprintf(json, "{\"id\": %zu,\"username\": \"%s\",\"password\": \"%s\",\"created\": \"%s\",\"modified\": \"%s\"}", u->id, u->username, u->password, created, modified);
    return json;
}