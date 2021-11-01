#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

typedef struct g_id
{
    int *id;
} t_id;

typedef struct g_philo
{
    t_id philo_id;
    int nbr_of_philos;
    int time_to_eat;
    int time_to_sleep;
    int time_to_die;
    pthread_t *profil;
    pthread_mutex_t *forks;

    // create var last_time_eat[i] = 
    long    *last_time_eat;
} t_philo;

t_philo *philos;


long time_now()
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    long time_mili = (current_time.tv_sec * 1000) + (current_time.tv_usec / 1000);
    return time_mili;
}
void *routine_philos(void *arg)
{
    int *id;
    id = (int *)arg;
    while (1)
    {
        pthread_mutex_lock(&philos->forks[*id - 1]);
        printf("philo %d take a fork L\n", *id);

        pthread_mutex_lock(&philos->forks[*id % philos->nbr_of_philos]);
        printf("philo %d take a fork R\n", *id);

        printf("philo %d is eating\n", *id);
        philos->last_time_eat[*id - 1] = time_now() ;
        usleep(philos->time_to_eat * 1000);


        pthread_mutex_unlock(&philos->forks[*id - 1]);
        pthread_mutex_unlock(&philos->forks[*id % philos->nbr_of_philos]);



        printf("philo %d is sleeping\n", *id);
        usleep(philos->time_to_eat * 1000);

        printf("philo %d is thinking\n", *id);
    }










    return NULL;
}


t_philo * get_data(t_philo *philos, int argc, char **argv)
{
    philos->nbr_of_philos = atoi(argv[1]); 
    philos->time_to_die = atoi(argv[2]); 
    philos->time_to_eat = atoi(argv[3]); 
    philos->time_to_sleep = atoi(argv[4]); 

   return philos;
}


int main(int argc, char **argv)
{
    int philo_is_dead;

    //t_philo *philos;
    philos = malloc(sizeof(t_philo ));
    philos = get_data(philos, argc, argv);
    philos->profil = malloc(sizeof(pthread_t) * philos->nbr_of_philos);
    philos->philo_id.id = malloc(sizeof(int) * philos->nbr_of_philos);
    philos->forks = malloc(sizeof(pthread_mutex_t) * philos->nbr_of_philos);
    philos->last_time_eat = malloc(sizeof(long) * philos->nbr_of_philos);



    
    int i = 0;
    while (i < philos->nbr_of_philos)
    {
        philos->philo_id.id[i]  = i + 1;
        pthread_mutex_init(&philos->forks[i], NULL);
        i++;
    }

    i = 0;
    while (i < philos->nbr_of_philos)
    {
        philos->last_time_eat[i] = time_now();
        pthread_create(&philos->profil[i], NULL, routine_philos, & philos->philo_id.id[i]);
        usleep(100);
        i++;
    }

    while (1)
    {
        // check if phili is die
        // check_life = time now - last time eat 
        // if check_life more then > time to die arg  ===> philo is die
        //int philo_is_dead;
        i = 0;
        while(i < philos->nbr_of_philos)
        {
            philo_is_dead = time_now() - philos->last_time_eat[i];
            if (philo_is_dead > philos->time_to_die)
            {
                pthread_mutex_t test;
                pthread_mutex_init(&test, NULL);
                pthread_mutex_lock(&test);
                printf("philos %d is dead\n", i + 1);
                return(1);
            }
            i++;
        }
        
    }


    i = 0;
    while (i < philos->nbr_of_philos)
    {
        pthread_join(philos->profil[i], NULL);
        i++;
    }

    return (0);
}
