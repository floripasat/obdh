/*
 * fec.c
 *
 * Copyright (C) 2004, Phil Karn
 * Copyright (C) 2017, Gabriel Mariano Marcelino
 * 
 * This file is part of FloripaSat-TTC
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>
 * 
 */

/**
 * \file fec.c
 * 
 * \brief Forward error correction implementation.
 * 
 * \author Phil Karn <karn@ka9q.net>; Mods. for FloripaSat-TTC by Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 13/04/2017
 * 
 * \addtogroup fec
 * \{
 */

#include <stdlib.h>
#include <string.h>

#include "fec.h"

void free_rs_char(RS *rs_ptr)
{
    free(rs_ptr->alpha_to);
    free(rs_ptr->index_of);
    free(rs_ptr->genpoly);
    free(rs_ptr);
}

void encode_rs_char(RS *rs_ptr, uint8_t *data, uint8_t *parity)
{
    //int16_t i, j;
    //uint8_t feedback;

    memset(parity, 0, rs_ptr->nroots*sizeof(uint8_t));
    
    // *******************************
    // -- TO FIX ---------------------
    // *******************************
    /*
    for(i=0; i<(int16_t)(rs_ptr->nn - rs_ptr->nroots - rs_ptr->pad); i++)
    {
        feedback = rs_ptr->index_of[data[i] ^ parity[0]];
        if (feedback != rs_ptr->nn) // feedback term is non-zero
        {
            for(j=1; j<rs_ptr->nroots; j++)
            {
                parity[j] ^= rs_ptr->alpha_to[modnn(rs_ptr, feedback + rs_ptr->genpoly[rs_ptr->nroots-j])];
            }
        }
        // Shift
        memmove(&parity[0], &parity[1], sizeof(uint8_t)*(rs_ptr->nroots-1));
        
        if (feedback != rs_ptr->nn)
        {
            parity[rs_ptr->nroots-1] = rs_ptr->alpha_to[modnn(rs_ptr, feedback + rs_ptr->genpoly[0])];
        }
        else
        {
            parity[rs_ptr->nroots-1] = 0;
        }
    }
    */
}

int16_t decode_rs_char(RS *rs_ptr, uint8_t *data, int16_t *eras_pos, int16_t no_eras)
{
    int16_t retval;
 
    int16_t deg_lambda, el, deg_omega;
    int16_t i, j, r, k;
    uint8_t u, q, tmp, num1, num2, den, discr_r;
    uint8_t lambda[rs_ptr->nroots + 1], s[rs_ptr->nroots];      // Err+Eras Locator poly and syndrome poly
    uint8_t b[rs_ptr->nroots + 1], t[rs_ptr->nroots + 1], omega[rs_ptr->nroots + 1];
    uint8_t root[rs_ptr->nroots], reg[rs_ptr->nroots + 1], loc[rs_ptr->nroots];
    int16_t syn_error, count;

    // form the syndromes; i.e., evaluate data(x) at roots of g(x)
    for(i=0; i<rs_ptr->nroots; i++)
    {
        s[i] = data[0];
    }

    for(j=1; j<(rs_ptr->nn - rs_ptr->pad); j++)
    {
        for(i=0; i<rs_ptr->nroots; i++)
        {
            if (s[i] == 0)
            {
                s[i] = data[j];
            }
            else
            {
                s[i] = data[j] ^ rs_ptr->alpha_to[modnn(rs_ptr, rs_ptr->index_of[s[i]] + (rs_ptr->fcr + i)*rs_ptr->prim)];
            }
        }
    }

    // Convert syndromes to index form, checking for nonzero condition
    syn_error = 0;
    for(i=0; i<rs_ptr->nroots; i++)
    {
        syn_error |= s[i];
        s[i] = rs_ptr->index_of[s[i]];
    }

    if (!syn_error)
    {
        /* if syndrome is zero, data[] is a codeword and there are no
        * errors to correct. So return data[] unmodified
        */
        count = 0;
        goto finish;
    }
    memset(&lambda[1], 0, rs_ptr->nroots*sizeof(lambda[0]));
    lambda[0] = 1;

    if (no_eras > 0)
    {
        // Init lambda to be the erasure locator polynomial
        lambda[1] = rs_ptr->alpha_to[modnn(rs_ptr, rs_ptr->prim*(rs_ptr->nn-1-eras_pos[0]))];
        for(i=1; i<no_eras; i++)
        {
            u = modnn(rs_ptr, rs_ptr->prim*(rs_ptr->nn - 1 - eras_pos[i]));
            for(j=i+1; j>0; j--)
            {
                tmp = rs_ptr->index_of[lambda[j - 1]];
                if (tmp != rs_ptr->nn)
                    lambda[j] ^= rs_ptr->alpha_to[modnn(rs_ptr, u + tmp)];
            }
        }
    }
    for(i=0; i<rs_ptr->nroots+1; i++)
        b[i] = rs_ptr->index_of[lambda[i]];

    // Begin Berlekamp-Massey algorithm to determine error+erasure locator polynomial
    r = no_eras;
    el = no_eras;
    while(++r <= rs_ptr->nroots)    // r is the step number
    {
        // Compute discrepancy at the r-th step in poly-form
        discr_r = 0;
        for(i=0; i<r; i++)
        {
            if ((lambda[i] != 0) && (s[r-i-1] != rs_ptr->nn))
            {
                discr_r ^= rs_ptr->alpha_to[modnn(rs_ptr, rs_ptr->index_of[lambda[i]] + s[r-i-1])];
            }
        }
        discr_r = rs_ptr->index_of[discr_r];    // Index form
        if (discr_r == rs_ptr->nn)
        {
            // 2 lines below: B(x) <-- x*B(x)
            memmove(&b[1], b, rs_ptr->nroots*sizeof(b[0]));
            b[0] = rs_ptr->nn;
        }
        else
        {
            // 7 lines below: T(x) <-- lambda(x) - discr_r*x*b(x)
            t[0] = lambda[0];
            for(i=0; i<rs_ptr->nroots; i++)
            {
                if(b[i] != rs_ptr->nn)
                    t[i+1] = lambda[i+1] ^ rs_ptr->alpha_to[modnn(rs_ptr, discr_r + b[i])];
                else
                    t[i+1] = lambda[i+1];
            }
            if (2 * el <= r + no_eras - 1)
            {
                el = r + no_eras - el;

                // 2 lines below: B(x) <-- inv(discr_r) * lambda(x)
                for(i=0; i<=rs_ptr->nroots; i++)
                {
                    b[i] = (lambda[i] == 0) ? rs_ptr->nn : modnn(rs_ptr, rs_ptr->index_of[lambda[i]] - discr_r + rs_ptr->nn);
                }
            }
            else
            {
                // 2 lines below: B(x) <-- x*B(x)
                memmove(&b[1], b, rs_ptr->nroots*sizeof(b[0]));
                b[0] = rs_ptr->nn;
            }
            memcpy(lambda, t, (rs_ptr->nroots + 1)*sizeof(t[0]));
        }
    }

    // Convert lambda to index form and compute deg(lambda(x))
    deg_lambda = 0;
    for(i=0; i<rs_ptr->nroots+1; i++)
    {
        lambda[i] = rs_ptr->index_of[lambda[i]];
        if (lambda[i] != rs_ptr->nn)
            deg_lambda = i;
    }
    // Find roots of the error+erasure locator polynomial by Chien search
    memcpy(&reg[1], &lambda[1], rs_ptr->nroots*sizeof(reg[0]));
    count = 0;      // Number of roots of lambda(x)
    for(i=1, k=rs_ptr->iprim-1; i<=rs_ptr->nn; i++, k=modnn(rs_ptr, k + rs_ptr->iprim))
    {
        q = 1;  // lambda[0] is always 0
        for(j=deg_lambda; j>0; j--)
        {
            if (reg[j] != rs_ptr->nn)
            {
                reg[j] = modnn(rs_ptr, reg[j] + j);
                q ^= rs_ptr->alpha_to[reg[j]];
            }
        }
        if (q != 0)
            continue;   // Not a root
        // store root (index-form) and error location number
        root[count] = i;
        loc[count] = k;
        /* If we've already found max possible roots,
        * abort the search to save time
        */
        if (++count == deg_lambda)
            break;
    }
    if (deg_lambda != count)
    {
        // deg(lambda) unequal to number of roots => uncorrectable error detected
        count = -1;
        goto finish;
    }

    // Compute err+eras evaluator poly omega(x) = s(x)*lambda(x) (modulo x**NROOTS). in index form. Also find deg(omega).
    deg_omega = deg_lambda-1;
    for(i=0; i<=deg_omega;i++)
    {
        tmp = 0;
        for(j=i; j>=0; j--)
        {
            if ((s[i - j] != rs_ptr->nn) && (lambda[j] != rs_ptr->nn))
                tmp ^= rs_ptr->alpha_to[modnn(rs_ptr, s[i - j] + lambda[j])];
        }
        omega[i] = rs_ptr->index_of[tmp];
    }

    // Compute error values in poly-form. num1 = omega(inv(X(l))), num2 = inv(X(l))**(FCR-1) and den = lambda_pr(inv(X(l))) all in poly-form
    for(j=count-1; j>=0; j--)
    {
        num1 = 0;
        for(i=deg_omega; i>=0; i--)
        {
            if (omega[i] != rs_ptr->nn)
                num1  ^= rs_ptr->alpha_to[modnn(rs_ptr, omega[i] + i * root[j])];
        }
        num2 = rs_ptr->alpha_to[modnn(rs_ptr, root[j] * (rs_ptr->fcr - 1) + rs_ptr->nn)];
        den = 0;

        // lambda[i+1] for i even is the formal derivative lambda_pr of lambda[i]
        for(i=MIN(deg_lambda, rs_ptr->nroots-1) & ~1; i>=0; i-=2)
        {
            if (lambda[i+1] != rs_ptr->nn)
                den ^= rs_ptr->alpha_to[modnn(rs_ptr, lambda[i+1] + i * root[j])];
        }

        // Apply error to data
        if (num1 != 0 && loc[j] >= rs_ptr->pad)
        {
            data[loc[j] - rs_ptr->pad] ^= rs_ptr->alpha_to[modnn(rs_ptr, rs_ptr->index_of[num1] + rs_ptr->index_of[num2] + rs_ptr->nn - rs_ptr->index_of[den])];
        }
    }
finish:
    if (eras_pos != (void*)0)
    {
        for(i=0; i<count; i++)
        {
            eras_pos[i] = loc[i];
        }
    }
    retval = count;
  
    return retval;
}

int16_t modnn(RS *rs_ptr, int16_t x)
{    
    while(x >= rs_ptr->nn)
    {
        x -= rs_ptr->nn;
        x = (x >> rs_ptr->mm) + (x & rs_ptr->nn);
    }
    
    return x;
}

//! \} End of fec implementation group
